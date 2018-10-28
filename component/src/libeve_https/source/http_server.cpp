#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define EVENT__HAVE_OPENSSL
#define EVENT_IN_DOXYGEN_

#ifdef _MSC_VER
#include <WinSock2.h>
#include <WS2tcpip.h>
#endif
#include <thread>
#include <sstream>
#include "openssl/ssl.h"
#include "openssl/err.h"
#include "http_server.h"
#include "uri_filter.h"

#include "event2/listener.h"
#include "event2/bufferevent.h"
#include "event2/bufferevent_ssl.h"
#include "event2/event.h"
#include "event2/http.h"
#include "event2/buffer.h"
#include "event2/util.h"
#include "event2/keyvalq_struct.h"

#ifndef INVALID_SOCKET
#define INVALID_SOCKET (int32_t)(~0)
#endif


namespace msf {

void acceptCallback(struct evconnlistener *listener, evutil_socket_t nfd, struct sockaddr *peer_sa, int peer_socklen, void *arg);

CHttpServer::CHttpServer(int32_t port, const Cert *cert /* = nullptr */)
	: m_port(port)
	, m_base(nullptr)
	, m_filter(new CUriFilter)
	, m_cert()
{
	if (cert != nullptr) {
		m_cert.setIsUsed(true);
		m_cert.caCertPath = cert->caCertPath;
		m_cert.serverCertPath = cert->serverCertPath;
		m_cert.serverPrivateKeyPath = cert->serverPrivateKeyPath;
		sslCommonSetup();
	}

	m_base = event_base_new();
	m_port = port;

	evutil_addrinfo hints = {0};
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = EVUTIL_AI_PASSIVE | EVUTIL_AI_ADDRCONFIG;
	char strport[NI_MAXSERV] = {0};
	evutil_snprintf(strport, sizeof(strport), "%d", m_port);
	evutil_addrinfo* ai = nullptr;
	evutil_getaddrinfo("0.0.0.0", strport, &hints, &ai);

	evutil_socket_t fd;
	fd = socket(ai->ai_family, SOCK_STREAM, 0);
	evutil_make_socket_nonblocking(fd);
	evutil_make_socket_closeonexec(fd);
	int32_t on = 1;
	setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, (const char*)&on, sizeof(on));
	evutil_make_listen_socket_reuseable(fd);
	bind(fd, ai->ai_addr, (ev_socklen_t)ai->ai_addrlen);
	evutil_freeaddrinfo(ai);

	listen(fd, 128);

	const int flags = LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_EXEC | LEV_OPT_CLOSE_ON_FREE;
	evconnlistener* listener = evconnlistener_new(m_base, nullptr, nullptr, flags, 0, fd);
	evconnlistener_set_cb(listener, acceptCallback, this);

	std::thread([](event_base* base) {
		event_base_dispatch(base);
	}, m_base).detach();
}

CHttpServer::~CHttpServer()
{
	if (m_base) {
		event_base_free(m_base);
		m_base = nullptr;
	}
	if (m_filter) {
		delete m_filter;
		m_filter = nullptr;
	}
}

void acceptCallback(struct evconnlistener *listener, evutil_socket_t nfd, struct sockaddr *peer_sa, int peer_socklen, void *arg)
{
	sockaddr_in* addr = (sockaddr_in*)peer_sa;
	BOOL isDone = FALSE;
	std::thread([&isDone, nfd, peer_sa, peer_socklen, arg]() {
		event_base* base = event_base_new();
		evhttp* http = evhttp_new(base);
		CHttpServer* server = reinterpret_cast<CHttpServer*>(arg);
		server->loadOpenssl(http);
		evhttp_get_request(http, nfd, peer_sa, peer_socklen);
		evhttp_set_gencb(http, CHttpServer::generalCallback, arg);
		isDone = TRUE;
		event_base_dispatch(base);
		evhttp_free(http);
		event_base_free(base);
	}).detach();

	while (!isDone) {
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}

static bool server_setup_certs(SSL_CTX *ctx
	, const char *cacert
	, const char *certificate_chain
	, const char *private_key)
{
	SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER, NULL);
	if (SSL_CTX_use_PrivateKey_file(ctx, private_key, SSL_FILETYPE_PEM) <= 0) {
		return false;
	}
	if (!SSL_CTX_load_verify_locations(ctx, cacert, NULL))
	{
		return false;
	}
	if (SSL_CTX_use_certificate_file(ctx, certificate_chain, SSL_FILETYPE_PEM) <= 0) {
		return false;
	}
	if (1 != SSL_CTX_check_private_key(ctx)) {
		return false;
	}
	return true;
}

static struct bufferevent* bevcb(struct event_base *base, void *arg)
{
	struct bufferevent* r;
	SSL_CTX *ctx = (SSL_CTX *)arg;

	r = bufferevent_openssl_socket_new(base,
		-1,
		SSL_new(ctx),
		BUFFEREVENT_SSL_ACCEPTING,
		BEV_OPT_CLOSE_ON_FREE);
	return r;
}

bool CHttpServer::loadOpenssl(evhttp *http)
{
	if (m_cert.isUsed() == false) return false;

	SSL_CTX *ctx = SSL_CTX_new(TLSv1_server_method());
	SSL_CTX_set_options(ctx,
		SSL_OP_SINGLE_DH_USE |
		SSL_OP_SINGLE_ECDH_USE |
		SSL_OP_NO_SSLv2);
	EC_KEY *ecdh = EC_KEY_new_by_curve_name(NID_X9_62_prime256v1);
	if (!ecdh) {
		return false;
	}
	if (1 != SSL_CTX_set_tmp_ecdh(ctx, ecdh)) {
		return false;
	}
	server_setup_certs(ctx, m_cert.caCertPath.c_str(), m_cert.serverCertPath.c_str(), m_cert.serverPrivateKeyPath.c_str());
	evhttp_set_bevcb(http, bevcb, ctx);

	return true;
}

void CHttpServer::subscribe(const std::string& uri, IHttpHandler* handler)
{
	handler->setServer(this);
	m_filter->subscribe(uri, handler);
}

void CHttpServer::unsubscribe(const std::string& uri)
{
	m_filter->unsubscribe(uri);
}

void CHttpServer::generalCallback(evhttp_request* request, void* context)
{	
	CHttpServer* server = reinterpret_cast<CHttpServer*>(context);
	server->handle(request);
	return;
}

void CHttpServer::connectionCloseCallback(evhttp_connection* connection, void* context)
{
}

static void *my_zeroing_malloc(size_t howmuch, const char *, int)
{
	return calloc(1, howmuch);
}

void CHttpServer::sslCommonSetup()
{
	//CRYPTO_set_mem_functions(my_zeroing_malloc, realloc, free);
	SSL_library_init();
	SSL_load_error_strings();
	OpenSSL_add_all_algorithms();
}

void CHttpServer::handle(evhttp_request* request)
{
	evhttp_connection* conn = evhttp_request_get_connection(request);
	event_base* base = evhttp_connection_get_base(conn);
	evhttp_connection_set_closecb(conn, connectionCloseCallback, this);

	const evhttp_uri* uri = evhttp_request_get_evhttp_uri(request);
	const char* path = evhttp_uri_get_path(uri);
	IHttpHandler* handler = m_filter->handler(path);
	if (handler) {
		handler->handle(request);
	}
	else {
		evhttp_send_reply(request, HTTP_NOTFOUND, nullptr, nullptr);
	}
	bufferevent* buffer = evhttp_connection_get_bufferevent(conn);
	bufferevent_enable(buffer, EV_READ);
}

}
