#include <iostream>
#include <string>
#include <map>
#include <thread>
#include <regex>
#include "ws_server/ws_server.h"
#include "ws_common/ws_request.h"
#include "ws_common/ws_response.h"

std::string replaceAll(const std::string &src, const std::string &srcSub, const std::string &dstSub)
{
	std::string s(src);
	size_t index = 0;
	while (1)
	{
		index = s.find(srcSub);
		if (index == -1) break;
		s.replace(index, srcSub.size(), "");
		s.insert(index, dstSub);
	}
	return s;
}

class CHandleTest : public websocket::CHandler
{
public:
	virtual int handle(websocket::CRequest *req, websocket::CResponse *res, websocket::CServer *server)
	{
		return 0;
	}
};

class CHandleHttpTest : public websocket::CHandler
{
public:
	virtual int handle(websocket::CRequest *req, websocket::CResponse *res, websocket::CServer *server)
	{
		res->setBody("hello");
		res->setStatus(websocket::CResponse::status::ok, "success");
		res->reply();
		std::this_thread::sleep_for(std::chrono::milliseconds(10 * 1000));
		return 0;
	}
};

class CHandleRegex : public websocket::CHandler
{
public:
	virtual int handle(websocket::CRequest *req, websocket::CResponse *res, websocket::CServer *server)
	{
		const auto &result = req->matchResult();
		std::cout << "uuid: " << result[0] << std::endl;
		std::cout << "id: " << result[1] << std::endl;
		return 0;
	}
};

class CHandleNtf : public websocket::CHandler
{
public:
	virtual int handle(websocket::CRequest *req, websocket::CResponse *res, websocket::CServer *server)
	{
		std::map<std::string, std::string> headers;
		headers.insert(std::make_pair("type", "alarm"));
		std::string topic("test");
		std::string body("ntf");
		server->pushBroadcastMessage(topic, &headers, &body, &headers);
		return 0;
	}
};

class CHandleSubscribe : public websocket::CHandler
{
public:
	virtual int handle(websocket::CRequest *req, websocket::CResponse *res, websocket::CServer *server)
	{
		server->subscribeBroadcast(req->header("topic"), req);
		// res->addHeader("__messageuuid__", req->header("__messageuuid__"));
		res->setBody("success\r\n");
		res->reply();
		return 0;
	}
};

class CHandleBroadcast : public websocket::CHandler
{
public:
	virtual int handle(websocket::CRequest *req, websocket::CResponse *res, websocket::CServer *server)
	{
		std::string body = "this is a broadcast message";
		server->pushBroadcastMessage(req->header("topic"), nullptr, &body, nullptr);
		return 0;
	}
};

int main()
{
#if 1
	websocket::CServer server;
	// websocket
	server.subscribe("/test", new CHandleTest(), websocket::CServer::ws);
	server.subscribe("/test/uuid/+/id/#", new CHandleRegex(), websocket::CServer::ws);
	server.subscribe("/ntf", new CHandleNtf(), websocket::CServer::ws);
	server.subscribe("/subscribe", new CHandleSubscribe(), websocket::CServer::ws);
	server.subscribe("/broadcast", new CHandleBroadcast(), websocket::CServer::ws);
	// http
	server.subscribe("/test", new CHandleHttpTest(), websocket::CServer::http);
	server.run(9002);
#endif

#if 0
	std::regex r("/a/[^/]*?/b/.*?");
	std::smatch result;
	std::string buf = "/a/1/b/12/12/12";
	bool b = std::regex_match(buf, result, r);

	std::string src = "/a/+/b/+/#";
	std::string &dst = replaceAll(src, "+", "[^/]*?");
	dst = replaceAll(dst, "#", ".*?");
#endif

	std::system("pause");

	return 0;
}
