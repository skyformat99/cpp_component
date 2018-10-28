#ifndef __HTTP_SERVER_H__
#define __HTTP_SERVER_H__
#include "base/basetype.h"
#include <mutex>
#include <list>
#include <string>
#include <map>

struct event_base;
struct evhttp;
struct evhttp_request;
struct evhttp_connection;

namespace msf {

class IHttpHandler;
class CUriFilter;

class CHttpServer
{
public:
	struct Cert
	{
		std::string caCertPath;
		std::string serverCertPath;
		std::string serverPrivateKeyPath;

		Cert()
			: caCertPath("")
			, serverCertPath("")
			, serverPrivateKeyPath("")
			, m_isUsed(false)
		{}

		friend class CHttpServer;
	private:
		void setIsUsed(bool isUsed) { m_isUsed = isUsed; }
		bool isUsed() { return m_isUsed; }

	private:
		bool m_isUsed;
	};

public:
	explicit CHttpServer(int32_t port, const Cert *cert = nullptr);
	virtual ~CHttpServer();
	virtual void subscribe(const std::string& uri, IHttpHandler* handler);
	virtual void unsubscribe(const std::string& uri);

protected:
	virtual void handle(evhttp_request* request);
public:
	static void generalCallback(evhttp_request* request, void* context);
	static void connectionCloseCallback(evhttp_connection* connection, void* context);
	bool loadOpenssl(evhttp *http);

private:
	void sslCommonSetup();

public:
	int32_t m_port;
	event_base* m_base;
	CUriFilter* m_filter;

private:
	Cert m_cert;
};

class IHttpHandler
{
	friend class CHttpServer;
public:
	explicit IHttpHandler() : m_master(nullptr) {}
	virtual ~IHttpHandler() {}
	virtual void handle(evhttp_request* request) = 0;
protected:
	inline CHttpServer* server() const { return m_master; }
private:
	inline void setServer(CHttpServer* server) { m_master = server; }
private:
	CHttpServer* m_master;
};

}

#endif //__HTTP_SERVER_H__
