#ifndef __COMMUNICATION_CC6F783B5ACE490A828BA5415CD36010_H__
#define __COMMUNICATION_CC6F783B5ACE490A828BA5415CD36010_H__

#include <string>
#include <map>
#include <set>

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include "url_parse.h"
#include "http_encode.h"

namespace websocket
{

class CRequest;
class CHandler;
class CServer;
class CCommunication
{
protected:
	typedef websocketpp::server<websocketpp::config::asio> server;

public:
	explicit CCommunication(CServer *server, int msgQueueMax);
	virtual ~CCommunication();

public:
	void subscribe(const std::string &topic, CHandler *handler);
	void unSubscribe(const std::string &topic);
	virtual void handleMessage(const websocketpp::connection_hdl &hdl, server::message_ptr msg) = 0;

protected:
	CHandler *findHandler(const std::string &topic);

protected:
	struct CMessageInfo
	{
		std::string topic;
		std::map<std::string, std::string> headers;
		std::map<std::string, std::string> params;
		std::string body;
	};

protected:
	CServer *m_wsServer;
	CUrlParse m_urlParse;
	int m_msgQueueMax;

private:
	std::map<std::string, CHandler*> m_handlers;
	std::mutex m_handlerMutex;
};

class CMsgCommunication : public CCommunication
{
public:
	explicit CMsgCommunication(CServer *server, int msgQueueMax);
	virtual ~CMsgCommunication();

public:
	void startBroadcastThread();
	void subscribeBroadcast(const std::string &topic, CRequest *req);
	void unSubscribeBroadcast(const std::string &topic, CRequest *req);
	void removeBroadcastSubscribe(const std::string &topic, websocketpp::connection_hdl hdl);
	void removeBroadcastSubscribe(websocketpp::connection_hdl hdl);
	void pushBroadcastMessage(
		const std::string &topic
		, const std::map<std::string, std::string> *headers
		, const std::string *body
		, const std::map<std::string, std::string> *params = nullptr);
	virtual void handleMessage(const websocketpp::connection_hdl &hdl, server::message_ptr msg);

private:
	void broadcastHandler();
	void broadcast(const std::set<websocketpp::connection_hdl, std::owner_less<websocketpp::connection_hdl>> &subscribers, const std::string &msg, websocketpp::frame::opcode::value type = websocketpp::frame::opcode::text);
	bool findSubscribers(const std::string &topic, std::set<websocketpp::connection_hdl, std::owner_less<websocketpp::connection_hdl>> &subscribers);

private:
	bool m_isRunBroadcast;
	std::thread m_broadcastThread;
	CUrlParse m_bcSubUrlParse;
	CHttpEncode m_httpEncode;
	std::map<std::string, std::set<websocketpp::connection_hdl, std::owner_less<websocketpp::connection_hdl>>> m_bcSubs;
	std::mutex m_bcSubsMutex;
	std::queue<CMessageInfo> m_msgQueue;
	std::mutex m_msgQueueMutex;
	std::condition_variable m_msgQueueCv;
};

class CHttpCommunication : public CCommunication
{
public:
	explicit CHttpCommunication(CServer *server, int msgQueueMax);
	virtual ~CHttpCommunication();

public:
	virtual void handleMessage(const websocketpp::connection_hdl &hdl, server::message_ptr msg);
};

}

#endif // __COMMUNICATION_CC6F783B5ACE490A828BA5415CD36010_H__
