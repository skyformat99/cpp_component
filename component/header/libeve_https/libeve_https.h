#ifndef __MRS_LIBEVENT_HTTP_H__
#define __MRS_LIBEVENT_HTTP_H__

#include <string>
#include <map>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <memory>
#include "http_server.h"

struct event_base;
struct evhttp;
struct evhttp_request;
struct evbuffer;
namespace http
{

const static int Get = 1;
const static int Post = 2;
const static int Head = 3;
const static int Put = 4;
const static int Delete = 5;
const static int Options = 6;
const static int Trace = 7;
const static int Connect = 8;
const static int Patch = 9;

class ILibeventHttpHandler;
class CLibeventHttpComm
{
public:
	explicit CLibeventHttpComm(evhttp_request *req);
	virtual ~CLibeventHttpComm();

public:
	evhttp_request *evreq() const { return m_pReq; }

	friend class ILibeventHttpHandler;
private:
	void setHandler(ILibeventHttpHandler *handler) { m_handler = handler; }

protected:
	evhttp_request *m_pReq;
	ILibeventHttpHandler *m_handler;
};

class CLibeventHttpRequest : public CLibeventHttpComm
{
public:
	explicit CLibeventHttpRequest(evhttp_request *req);
	virtual ~CLibeventHttpRequest();

public:
	void peer(std::string &ip, uint16_t &port);
	const char *uri();
	std::string path();
	const char *host();
	short port();
	int method();
	int responseCode();
	const char *responceDesc();
	void getUrlParam(const std::string &key, std::string &value);
	void getUrlParams(const std::vector<std::string> &keys, std::map<std::string, std::string> &kvs);
	void getHeaders(std::map<std::string, std::string> &headers);
	std::string getHeader(const std::string &key, const std::map<std::string, std::string> &headers);
	void setBufLen(int len);
	// single
	std::string body();
	// mul
	const char *bodyNext(int &len);

	// copy avoidence
	void bodyData(std::function<char**(size_t len)> func);

	void destroyBuf(const char **buf);
	// unique_ptr mul
	std::unique_ptr<char[]> autoBody(int &len);
	// download file
	bool downloadFile(const std::string &root, std::string &filename, const char *download_isend = "isend", const char *download_context = "context", const char *download_fileext = "ext");
	// put file
	void addPutfileSession(const std::string &context, const std::string &filepath);
	void putFile(int code, const std::string &desc, const char *put_context = "context", const char *put_buflen = "buflen", const char *put_rellen = "rellen", const char *put_isend = "isend");

private:
	void writeFile(const std::string &path);

protected:
	evbuffer *m_buf;
private:
	bool m_isEnd;
	int m_bufLen;
};


class CLibeventHttpResponse : public CLibeventHttpComm
{
public:
	explicit CLibeventHttpResponse(evhttp_request *req);
	virtual ~CLibeventHttpResponse();

public:
	void setCode(int code) { m_code = code; }
	const int &code() const { return m_code; }
	void setDesc(const std::string &desc) { m_desc = desc; }
	const std::string &desc() { return m_desc; }
	void addHeader(const std::string &key, const std::string &value);
	void removeHeader(const std::string &key);
	void addHeaders(const std::map<std::string, std::string> &kvs);
	void write(const std::string &body);
	void write(const char *body, int len);
	void redirection(const char *newUrl, bool isForever = false);

private:
	int m_code;
	std::string m_desc;
};

class CLibeventHttp;
class ILibeventHttpHandler : public msf::IHttpHandler
{
public:
	explicit ILibeventHttpHandler()
		: m_context(nullptr) {}
	virtual ~ILibeventHttpHandler() {}
	
public:
	virtual void handle(CLibeventHttpRequest *req, CLibeventHttpResponse *res) = 0;
	void handle(evhttp_request* req) final
	{
		CLibeventHttpRequest request(req);
		CLibeventHttpResponse response(req);
		request.setHandler(this);
		response.setHandler(this);
		handle(&request, &response);
	}
public:
	void *context() const { return m_context; }
	void setContext(void *context) { m_context = context; }
	void setServer(CLibeventHttp *server) { m_server = server; }
	CLibeventHttp *server() { return m_server; }

private:
	void *m_context;
	CLibeventHttp *m_server;
};


class CLibeventHttp : public msf::CHttpServer
{
public:
	typedef msf::CHttpServer::Cert Cert;

public:
	explicit CLibeventHttp(uint32_t port, const Cert *cert = nullptr);
	virtual ~CLibeventHttp();

private:
	typedef std::function<void(CLibeventHttpRequest *req, CLibeventHttpResponse *res)> handlefunc;

public:
	void handle(const std::string &strPath, ILibeventHttpHandler *handler);
	void remove(const std::string &strPath);
	void setTimeout(long sec, long us);
#if 0
	void handle(const std::string &strPath, handlefunc f);
	void startListen(const std::string &strIp, const short u16Port);

private:
	static void generic_handler(evhttp_request *req, void *arg);
	void onHandler(evhttp_request *req);
	ILibeventHttpHandler *getHandler(const std::string &strPath);
	std::tuple<bool, handlefunc> getFunHandler(const std::string &strPath);
#endif

	friend class CLibeventHttpRequest;
private:
	struct FileInfo
	{
		FileInfo()
			: mutex(), filepath(), offset(0) {}
		~FileInfo() {}

		std::mutex mutex;
		std::string filepath;
		int offset;
	};

private:
	event_base *m_httpBase;
	evhttp *m_http;
	std::map<std::string, ILibeventHttpHandler*> m_paths;
	std::map<std::string, handlefunc> m_pathfuncs;
	std::mutex m_mutex;
	std::condition_variable m_cv;
	std::mutex m_thread_mutex;
	std::map<std::string, FileInfo*> m_fileMapping;
	std::mutex m_fileMutex;
};

}

#endif
