#include <fstream>
#include "libeve_https.h"
#include "event2/event.h"
#include "event2/buffer.h"
#include "event2/http.h"
#include "evhttp.h"
#include "ospathtool/ospathtool.h"
#include <string.h>
//#include "compat/sys/queue.h"
#include <iostream>
#ifdef WIN32
#include <Winsock2.h>
#endif

namespace http
{

#ifdef WIN32
int init_win_socket()
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		return -1;
	}
	return 0;
}
#endif

CLibeventHttp::CLibeventHttp(uint32_t port, const Cert *cert /* = nullptr */)
	: msf::CHttpServer(port, cert)
	, m_httpBase(nullptr)
	, m_http(nullptr)
	, m_paths()
	, m_pathfuncs()
	, m_mutex()
	, m_cv()
	, m_thread_mutex()
	, m_fileMapping()
	, m_fileMutex()
{
#ifdef WIN32
	init_win_socket();
#endif
	m_httpBase = event_base_new();
	m_http = evhttp_new(m_httpBase);
}

CLibeventHttp::~CLibeventHttp()
{
	evhttp_free(m_http);
#ifdef WIN32
	WSACleanup();
#endif
}

#if 0
void CLibeventHttp::startListen(const std::string &strIp, const short u16Port)
{
	int ret = evhttp_bind_socket(m_http, strIp.c_str(), u16Port);
	event_base_dispatch(m_httpBase);
}

void CLibeventHttp::generic_handler(evhttp_request *req, void *arg)
{
	CLibeventHttp *http = reinterpret_cast<CLibeventHttp*>(arg);
	if (http == nullptr) return;
	http->onHandler(req);
}

void CLibeventHttp::onHandler(evhttp_request *req)
{
	const evhttp_uri *uri = evhttp_request_get_evhttp_uri(req);
	if (uri == nullptr) return;
	const char *path = evhttp_uri_get_path(uri);
	if (path == nullptr) return;
	ILibeventHttpHandler *handler = getHandler(path);
	CLibeventHttpRequest request(req);
	CLibeventHttpResponse response(req);
	if (handler != nullptr)
	{
		handler->handle(&request, &response);
	}
	else
	{
		std::tuple<bool, handlefunc> t = getFunHandler(path);
		bool isFind = std::get<0>(t);
		if (isFind == false) return;
		handlefunc f = std::get<1>(t);
		f(&request, &response);
	}
	std::thread([this, handler, req] {
		std::unique_lock<std::mutex> lk(m_thread_mutex);
		if (m_count > m_maxConnectNum)
		{
			m_cv.wait(lk);
		}
		++m_count;
		lk.unlock();
		CLibeventHttpRequest request(req);
		CLibeventHttpResponse response(req);
		handler->handle(&request, &response);
		lk.lock();
		--m_count;
		lk.unlock();
		m_cv.notify_all();
	}).detach();
}
#endif

void CLibeventHttp::handle(const std::string &strPath, ILibeventHttpHandler *handler)
{
	subscribe(strPath, handler);
	handler->setServer(this);
#if 0
	std::unique_lock<std::mutex> lk(m_mutex);
	m_paths.insert(std::make_pair(strPath, handler));
	lk.unlock();
	evhttp_set_cb(m_http, strPath.c_str(), generic_handler, this);
#endif
}

#if 0
void CLibeventHttp::handle(const std::string &strPath, handlefunc f)
{
	std::unique_lock<std::mutex> lk(m_mutex);
	m_pathfuncs.insert(std::make_pair(strPath, f));
	lk.unlock();
	evhttp_set_cb(m_http, strPath.c_str(), generic_handler, this);
}
#endif

void CLibeventHttp::remove(const std::string &strPath)
{
	unsubscribe(strPath);
#if 0
	std::unique_lock<std::mutex> lk(m_mutex);
	auto iter1 = m_paths.find(strPath);
	if (iter1 != m_paths.end())
	{
		m_paths.erase(iter1);
	}
	auto iter2 = m_pathfuncs.find(strPath);
	if (iter2 != m_pathfuncs.end())
	{
		m_pathfuncs.erase(iter2);
	}
	lk.unlock();
	evhttp_del_cb(m_http, strPath.c_str());
#endif
}

void CLibeventHttp::setTimeout(long sec, long us)
{
	timeval tv = { 0 };
	tv.tv_sec = sec;
	tv.tv_usec = us;
	evhttp_set_timeout_tv(m_http, &tv);
}

#if 0
ILibeventHttpHandler *CLibeventHttp::getHandler(const std::string &strPath)
{
	ILibeventHttpHandler *handler = nullptr;
	std::unique_lock<std::mutex> lk(m_mutex);
	auto iter = m_paths.find(strPath);
	if (iter != m_paths.end())
	{
		handler = iter->second;
	}
	lk.unlock();
	return handler;
}

std::tuple<bool, CLibeventHttp::handlefunc> CLibeventHttp::getFunHandler(const std::string &strPath)
{
	bool isFind = false;
	handlefunc f;
	std::unique_lock<std::mutex> lk(m_mutex);
	auto iter = m_pathfuncs.find(strPath);
	if (iter != m_pathfuncs.end())
	{
		isFind = true;
		f = iter->second;
	}
	lk.unlock();
	return std::make_tuple(isFind, f);
}
#endif


//////////////////////////////////
CLibeventHttpComm::CLibeventHttpComm(evhttp_request *req)
	: m_pReq(req)
	, m_handler(nullptr)
{
}

CLibeventHttpComm::~CLibeventHttpComm()
{
}


//////////////////////////////////
CLibeventHttpRequest::CLibeventHttpRequest(evhttp_request *req)
	: CLibeventHttpComm(req)
	, m_buf(nullptr)
	, m_isEnd(true)
	, m_bufLen(8192)
{
}

CLibeventHttpRequest::~CLibeventHttpRequest()
{
}

void CLibeventHttpRequest::peer(std::string &ip, uint16_t &port)
{
	evhttp_connection *conn = evhttp_request_get_connection(m_pReq);
	char *p = nullptr;
	evhttp_connection_get_peer(conn, &p, &port);
	ip = p;
}

const char *CLibeventHttpRequest::uri()
{
	return evhttp_request_get_uri(m_pReq);
}

std::string CLibeventHttpRequest::path()
{
	const evhttp_uri *uri = evhttp_request_get_evhttp_uri(m_pReq);
	if (uri == nullptr) return "";
	const char *path = evhttp_uri_get_path(uri);
	char *decoded_uri = evhttp_decode_uri(path);
	std::string urlPath = decoded_uri;
	free(decoded_uri);
	return urlPath;
}

const char *CLibeventHttpRequest::host()
{
	return evhttp_request_get_host(m_pReq);
}

short CLibeventHttpRequest::port()
{
	const evhttp_uri *uri = evhttp_request_get_evhttp_uri(m_pReq);
	return evhttp_uri_get_port(uri);
}

int CLibeventHttpRequest::method()
{
	return evhttp_request_get_command(m_pReq);
}

int CLibeventHttpRequest::responseCode()
{
	return evhttp_request_get_response_code(m_pReq);
}

const char *CLibeventHttpRequest::responceDesc()
{
	return evhttp_request_get_response_code_line(m_pReq);
}

void CLibeventHttpRequest::getUrlParam(const std::string &key, std::string &value)
{
	const char *uri = evhttp_request_get_uri(m_pReq);
	// encode chain
	uri = evhttp_encode_uri(uri);
	char *decoded_uri = evhttp_decode_uri(uri);
	evkeyvalq params = { 0 };
	evhttp_parse_query(decoded_uri, &params);
	const char *v = evhttp_find_header(&params, key.c_str());
	free(decoded_uri);
	if (v == nullptr) return;
	value = v;
}

void CLibeventHttpRequest::getUrlParams(const std::vector<std::string> &keys, std::map<std::string, std::string> &kvs)
{
	const char *uri = evhttp_request_get_uri(m_pReq);
	char *decoded_uri = evhttp_decode_uri(uri);
	evkeyvalq params = { 0 };
	evhttp_parse_query(decoded_uri, &params);
	kvs.clear();
	for (auto iter = keys.begin(); iter != keys.end(); ++iter)
	{
		const char *v = evhttp_find_header(&params, (*iter).c_str());
		if (v == nullptr) continue;
		kvs.insert(std::make_pair(*iter, v));
	}
	free(decoded_uri);
}

void CLibeventHttpRequest::getHeaders(std::map<std::string, std::string> &headers)
{
	headers.clear();
	evkeyvalq *hs = evhttp_request_get_input_headers(m_pReq);
	for (evkeyval *h = hs->tqh_first; h; h = h->next.tqe_next)
	{
		headers.insert(std::make_pair(h->key, h->value));
	}
}

std::string CLibeventHttpRequest::getHeader(const std::string &key, const std::map<std::string, std::string> &headers)
{
	auto iter = headers.find(key);
	if (iter != headers.end())
	{
		return iter->second;
	}
	return "";
}

void CLibeventHttpRequest::setBufLen(int len)
{
	m_bufLen = len;
}

std::string CLibeventHttpRequest::body()
{
	evbuffer *buf = evhttp_request_get_input_buffer(m_pReq);
	size_t size = evbuffer_get_length(buf) + 1;
	char *b = new char[size];
	memset(b, 0, size);
	evbuffer_remove(buf, b, size);
	std::string tmp(b);
	delete[] b;
	b = nullptr;
	return tmp;
}

const char *CLibeventHttpRequest::bodyNext(int &len)
{
	if (m_buf == nullptr)
	{
		m_buf = evhttp_request_get_input_buffer(m_pReq);
	}
	if (m_buf == nullptr) return nullptr;
	size_t size = evbuffer_get_length(m_buf);
	if (size == 0)
	{
		//evbuffer_free(m_buf);
		m_buf = nullptr;
		return nullptr;
	}
	char *buf = new char[m_bufLen];
	memset(buf, 0, m_bufLen);
	int n = evbuffer_remove(m_buf, buf, m_bufLen);
	if (n == 0) return nullptr;
	len = n;
	return buf;
}

void CLibeventHttpRequest::bodyData(std::function<char**(size_t len)> func)
{
	evbuffer *buf = evhttp_request_get_input_buffer(m_pReq);
	size_t size = evbuffer_get_length(buf);
	char** des = nullptr;
	des = func(size);
	evbuffer_remove(buf, *des, size);
}

void CLibeventHttpRequest::destroyBuf(const char **buf)
{
	if (*buf != nullptr)
	{
		delete[] *buf;
		*buf = nullptr;
	}
}

std::unique_ptr<char[]> CLibeventHttpRequest::autoBody(int &len)
{
	const char *buf = bodyNext(len);
	if (buf == nullptr) {
		return std::unique_ptr<char[]>(nullptr);
	}
	std::unique_ptr<char[]> ptr = std::unique_ptr<char[]>(const_cast<char*>(bodyNext(len)));
	return ptr;
}

void CLibeventHttpRequest::addPutfileSession(const std::string &context, const std::string &filepath)
{
	CLibeventHttp *server = static_cast<CLibeventHttp*>(m_handler->server());
	server->m_fileMutex.lock();
	auto iter = server->m_fileMapping.find(context);
	if (iter == server->m_fileMapping.end()) {
		CLibeventHttp::FileInfo *info = new CLibeventHttp::FileInfo();
		info->filepath = filepath;
		server->m_fileMapping.insert(std::make_pair(context, info));
	}
	server->m_fileMutex.unlock();
}

void CLibeventHttpRequest::putFile(int code, const std::string &desc, const char *put_context /* = "context" */, const char *put_buflen /* = "buflen" */, const char *put_rellen /* = "rellen" */, const char *put_isend /* = "isend" */)
{
	CLibeventHttp *server = static_cast<CLibeventHttp*>(m_handler->server());
	std::map<std::string, std::string> headers;
	getHeaders(headers);
	const std::string &context = getHeader(put_context, headers);
	const std::string &bufLenStr = getHeader(put_buflen, headers);
	int bufLen = std::stoi(bufLenStr);
	auto iter = server->m_fileMapping.find(context);
	if (iter == server->m_fileMapping.end()) return;
	iter->second->mutex.lock();
	std::string isend = "0";
	do
	{
		const std::string &filepath = iter->second->filepath;
		int len = iter->second->offset;
		std::ifstream readFile(filepath, std::ios::binary);
		if (!readFile) break;
		readFile.seekg(len, std::ios::cur);
		char *buf = new char[bufLen];
		memset(buf, 0, bufLen);
		readFile.read(buf, bufLen);
		int n = static_cast<int>(readFile.gcount());
		if (n <= 0) {
			iter->second->mutex.unlock();
			if (iter->second != nullptr) {
				CLibeventHttp::FileInfo *info = iter->second;
				server->m_fileMutex.lock();
				server->m_fileMapping.erase(iter);
				server->m_fileMutex.unlock();
				delete info;
				info = nullptr;
			}
			readFile.close();
			isend = "1";
			CLibeventHttpResponse response(m_pReq);
			response.addHeader(put_isend, isend);
			response.addHeader(put_rellen, std::to_string(n));
			response.setCode(code);
			response.setDesc(desc);
			response.write(buf, n);
			return;
		}
		readFile.close();
		CLibeventHttpResponse response(m_pReq);
		response.addHeader(put_rellen, std::to_string(n));
		response.addHeader(put_isend, isend);
		response.setCode(code);
		response.setDesc(desc);
		response.write(buf, n);
		iter->second->offset += n;
	} while (0);
	iter->second->mutex.unlock();
}

bool CLibeventHttpRequest::downloadFile(const std::string &root, std::string &filename, const char *download_isend, const char *download_context, const char *download_fileext)
{
	ospathtool::COsPath ospath;
	if (ospath.exists(root) == false) {
		ospathtool::COs os;
		os.mkdirs(root);
	}
	std::map<std::string, std::string> headers;
	getHeaders(headers);
	std::string isend = getHeader(download_isend, headers);
	if (isend == "") return true;
	std::string context = getHeader(download_context, headers);
	if (context == "") return true;
	std::string ext = getHeader(download_fileext, headers);
	std::string r = root;
	if (root.back() != '/') {
		r.append("/");
	}
	filename = context.append(ext);
	std::string filepath = r + filename;
	if (isend == std::string("1"))
	{
		return true;
	}
	else
	{
		writeFile(filepath);
	}
	return false;
}

void CLibeventHttpRequest::writeFile(const std::string &path)
{
	ospathtool::COs os;
	ospathtool::COsPath ospath;
	std::string dir;
	bool b = ospath.dirname(path, dir);
	if (b == true)
	{
		bool exist = ospath.exists(dir);
		if (exist == false)
		{
			os.mkdirs(dir);
		}
	}
	std::ofstream file(path, std::ios::binary|std::ios::app);
	const char *buf = nullptr;
	int len = 0;
	while (buf = bodyNext(len))
	{
		if (buf == nullptr) break;
		file.write(buf, len);
		destroyBuf(&buf);
	}
	file.close();
}


//////////////////////////////////
CLibeventHttpResponse::CLibeventHttpResponse(evhttp_request *req)
	: CLibeventHttpComm(req)
{
}

CLibeventHttpResponse::~CLibeventHttpResponse()
{
}

void CLibeventHttpResponse::addHeader(const std::string &key, const std::string &value)
{
	evkeyvalq *evkv = evhttp_request_get_output_headers(m_pReq);
	evhttp_add_header(evkv, key.c_str(), value.c_str());
}

void CLibeventHttpResponse::addHeaders(const std::map<std::string, std::string> &kvs)
{
	evkeyvalq *evkv = evhttp_request_get_output_headers(m_pReq);
	for (auto iter = kvs.begin(); iter != kvs.end(); ++iter)
	{
		evhttp_add_header(evkv, iter->first.c_str(), iter->second.c_str());
	}
}

void CLibeventHttpResponse::removeHeader(const std::string &key)
{
	evkeyvalq *evkv = evhttp_request_get_output_headers(m_pReq);
	evhttp_remove_header(evkv, key.c_str());
}

void CLibeventHttpResponse::write(const std::string &body)
{
	write(body.c_str(), static_cast<int>(body.length()));
}

void CLibeventHttpResponse::write(const char *body, int len)
{
	struct evbuffer *buf = evbuffer_new();
	if (buf == nullptr) return;
	evbuffer_add(buf, body, len);
	evhttp_send_reply(m_pReq, m_code, m_desc.c_str(), buf);
	evbuffer_free(buf);
}

void CLibeventHttpResponse::redirection(const char *newUrl, bool isForever /* = false */)
{
	addHeader("Location", newUrl);
	if (isForever == true) {
		setCode(301);
	}
	else {
		setCode(302);
	}
	setDesc("redirection");
	write("");
}


}
