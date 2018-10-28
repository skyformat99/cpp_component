#include "randtool/uuid.h"
#include "libeve_https/libeve_https.h"
#include <iostream>
#include <fstream>

#ifdef WIN32
#include <Winsock2.h>
#endif
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

class CHandleIndex : public http::ILibeventHttpHandler
{
public:
	virtual void handle(http::CLibeventHttpRequest *req, http::CLibeventHttpResponse *res)
	{
#if 0
		std::string body = req->body();
		std::cout << body << std::endl;
		int len = 0;
		std::cout << "body: " << req->autoBody(len).get() << std::endl;
#endif
		std::string peerIp("");
		uint16_t port = 0;
		req->peer(peerIp, port);
		res->setCode(200);
		res->setDesc("success");
		std::string response = "index";
		// res->write(response);
		res->write(response.c_str(), (int)response.size());
#if 0
#endif
	}
};

class CHandleHello : public http::ILibeventHttpHandler
{
public:
	virtual void handle(http::CLibeventHttpRequest *req, http::CLibeventHttpResponse *res)
	{
		std::string testValue;
		req->getUrlParam("test", testValue);
		std::cout << testValue << std::endl;
		res->setCode(200);
		res->setDesc("success");
		res->write("hello\n");
	}
};

class CHandleUploadFile : public http::ILibeventHttpHandler
{
public:
	virtual void handle(http::CLibeventHttpRequest *req, http::CLibeventHttpResponse *res)
	{
		std::string filename;
		req->downloadFile("upload", filename);
		res->setCode(200);
		res->setDesc("success");
		res->write("");
	}
};

class CHandleStartDownload : public http::ILibeventHttpHandler
{
public:
	virtual void handle(http::CLibeventHttpRequest *req, http::CLibeventHttpResponse *res)
	{
		randtool::CUuid logictool;
		const std::string &uuid = logictool.generator();
		req->addPutfileSession(uuid, "./file/test.jpg");
		res->setCode(200);
		res->setDesc("success");
		res->write(uuid);
	}
};

class CHandleDownloadFile : public http::ILibeventHttpHandler
{
public:
	virtual void handle(http::CLibeventHttpRequest *req, http::CLibeventHttpResponse *res)
	{
		req->putFile(200, "success");
	}
};

class CHandleRedirSrc : public http::ILibeventHttpHandler
{
public:
	virtual void handle(http::CLibeventHttpRequest *req, http::CLibeventHttpResponse *res)
	{
		const std::string &body = req->body();
		res->redirection("http://192.168.9.15:8080/redir/dst");
		// res->redirection("https://www.baidu.com");
	}
};

class CHandleRedirDst : public http::ILibeventHttpHandler
{
public:
	virtual void handle(http::CLibeventHttpRequest *req, http::CLibeventHttpResponse *res)
	{
		const std::string &body = req->body();
		res->setCode(200);
		res->setDesc("success");
		res->write("success");
	}
};

class CHandleMultiFile : public http::ILibeventHttpHandler
{
public:
	virtual void handle(http::CLibeventHttpRequest *req, http::CLibeventHttpResponse *res)
	{
		const std::string &body = req->body();
		std::ofstream ss("body.txt");
		ss << body;
		ss.flush();
		std::map<std::string, std::string> reqHeaders;
		req->getHeaders(reqHeaders);
		res->setCode(200);
		res->setDesc("success");
		res->write("success");
	}
};

class CHandleHttps : public http::ILibeventHttpHandler
{
public:
	virtual void handle(http::CLibeventHttpRequest *req, http::CLibeventHttpResponse *res)
	{
		res->setCode(200);
		res->setDesc("success");
		res->write("hello");
	}
};

int main()
{
#ifdef WIN32
	init_win_socket();
#endif
#if 1
	http::CLibeventHttp http(8080);
	CHandleIndex *index = new CHandleIndex();
	CHandleHello *hello = new CHandleHello();
	CHandleUploadFile *upload = new CHandleUploadFile();
	CHandleStartDownload *startDownload = new CHandleStartDownload();
	CHandleDownloadFile *download = new CHandleDownloadFile();
	CHandleRedirSrc *redirSrc = new CHandleRedirSrc();
	CHandleRedirDst *redirDst = new CHandleRedirDst();
	CHandleMultiFile *multiFile = new CHandleMultiFile();
	http.handle("/", index);
	http.handle("/hello", hello);
	http.handle("/upload", upload);
	http.handle("/start/download", startDownload);
	http.handle("/download", download);
	http.handle("/redir/src", redirSrc);
	http.handle("/redir/dst", redirDst);
	http.handle("/multi/file", multiFile);
#endif

#if 0
	http::CLibeventHttp::Cert cert;
	cert.caCertPath = "ca-cert.pem";
	cert.serverCertPath = "server-cert.pem";
	cert.serverPrivateKeyPath = "server-key.pem";
	http::CLibeventHttp http2(8581, &cert);
	CHandleHttps *https = new CHandleHttps();
	http2.handle("/https", https);
#endif
#if 0
	for (int i = 0; i < 10; ++i) {
		std::thread([] {
			http::CLibeventHttp http(8080);
			CHandleIndex *index = new CHandleIndex();
			CHandleHello *hello = new CHandleHello();
			http.handle("/", index);
			http.handle("/hello", hello);
			http.handle("/test", [](http::CLibeventHttpRequest *req, http::CLibeventHttpResponse *rsp) {
				rsp->setCode(200);
				rsp->setDesc("success");
				rsp->write("test");
			});
			http.startListen("192.168.9.15", 8080);
		}).detach();
	}
#endif

	std::system("pause");
#ifdef WIN32
	WSACleanup();
#endif

	return 0;
}
