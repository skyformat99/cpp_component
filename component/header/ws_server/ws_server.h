#ifndef __WS_SERVER_A04353273AF042C5A588BB3EBD251FBE_H__
#define __WS_SERVER_A04353273AF042C5A588BB3EBD251FBE_H__

#include <stdint.h>
#include <string>
#include <map>
#include <mutex>
#include <list>
#include <set>
#include <queue>
#include <condition_variable>

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include "ws_common/ws_request.h"
#include "ws_common/ws_response.h"

namespace websocket
{

class CUrlParse;
class CServer;
class CHttpEncode;
class CCommunication;
class CMsgCommunication;
class CHttpCommunication;
class CHandler
{
public:
	virtual int handle(CRequest *req, CResponse *res, CServer *server) = 0;

	friend class CCommunication;
	friend class CServer;
private:
	void setServer(CServer *server) { m_server = server; }

private:
	CServer *m_server;
};

class CServer
{
public:
	enum communication_type
	{
		http = 1,
		ws = 2
	};

private:
	typedef websocketpp::server<websocketpp::config::asio> server;

public:
	explicit CServer(int msgQueueMax = 100);
	virtual ~CServer();

public:
	void run(uint16_t port);
	void subscribe(const std::string &topic, CHandler *handler, communication_type type);
	void unSubscribe(const std::string &topic, communication_type type);
	void subscribeBroadcast(const std::string &topic, CRequest *req);
	void unSubscribeBroadcast(const std::string &topic, CRequest *req);
	void pushBroadcastMessage(
		const std::string &topic
		, const std::map<std::string, std::string> *headers
		, const std::string *body
		, const std::map<std::string, std::string> *params = nullptr);

private:
	void onMessage(websocketpp::connection_hdl h, server::message_ptr msg);
	void onHttp(websocketpp::connection_hdl hdl);
	void onOpen(websocketpp::connection_hdl hdl);
	void onClose(websocketpp::connection_hdl hdl);

private:
	void addConnect(websocketpp::connection_hdl hdl);
	void removeConnect(websocketpp::connection_hdl hdl);
	CCommunication *communication(communication_type type);

private:
	struct CMessageInfo
	{
		std::string topic;
		std::map<std::string, std::string> headers;
		std::map<std::string, std::string> params;
		std::string body;
	};

	friend class CCommunication;
	friend class CMsgCommunication;
	friend class CHttpCommunication;
private:
	server &getServer() { return m_server; }

private:
	server m_server;
	std::set<websocketpp::connection_hdl, std::owner_less<websocketpp::connection_hdl>> m_connSet;
	std::mutex m_connMutex;
	CMsgCommunication *m_msgCommunication;
	CHttpCommunication *m_httpCommunication;
};

}

#endif // __WS_CLIENT_A04353273AF042C5A588BB3EBD251FBE_H__
