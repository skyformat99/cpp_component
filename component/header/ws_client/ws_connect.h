#ifndef __WS_CONNECT_84E56D497D3341A197295DF5FC501D95_H__
#define __WS_CONNECT_84E56D497D3341A197295DF5FC501D95_H__

#include <stdint.h>
#include <string>
#include <map>
#include <mutex>
#include <list>
#include <functional>
#include <condition_variable>

#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>
#include <websocketpp/common/thread.hpp>
#include <websocketpp/common/memory.hpp>

namespace websocket
{

class CTools;
class CConnect
{
public:
	typedef websocketpp::client<websocketpp::config::asio_client> client;

private:
	typedef std::function<void(const std::string &msg)> msgCallback;

public:
	explicit CConnect(const std::string &url, int connTimeoutMS = 1000 * 30);
	virtual ~CConnect();

public:
	void setMessageCallback(msgCallback f);
	int asyncSend(const std::string &method, const std::string &uri
		, const std::string &version
		, const std::map<std::string, std::string> *params
		, const std::map<std::string, std::string> *headers
		, const std::string &body
		, websocketpp::frame::opcode::value type = websocketpp::frame::opcode::text);
	int send(const std::string *method, const std::string *uri
		, const std::string *version
		, const std::map<std::string, std::string> *params
		, const std::map<std::string, std::string> *headers
		, const std::string *body
		, std::string *response
		, int sendTimeoutMS = 30 * 1000
		, websocketpp::frame::opcode::value type = websocketpp::frame::opcode::text);

public:
	operator bool()
	{
		return m_connFlag;
	}

private:
	void onMessage(websocketpp::connection_hdl h, client::message_ptr msg);
	void onOpen(client *c, websocketpp::connection_hdl hdl);

private:
	std::string joinRaw(const std::string *method
		, const std::string *uri
		, const std::string *version
		, const std::map<std::string, std::string> *params
		, const std::map<std::string, std::string> *headers
		, const std::string *body, std::string &raw, bool isAddMsgUuid = false);
	void joinParams(const std::string &uri, const std::map<std::string, std::string> &params, std::string &str);
	void handleSyncMessage(const std::string &messageUuid, const std::string &msg);

private:
	struct CMessage
	{
		std::mutex mutex;
		std::condition_variable cv;
		std::string response;
	};

private:
	client m_client;
	client::connection_ptr m_conn;
	std::mutex m_connMutex;
	std::condition_variable m_connCv;
	bool m_connFlag;
	int m_connTimeoutMS;
    websocketpp::lib::shared_ptr<websocketpp::lib::thread> m_thread;
	std::map<std::string, CMessage*> m_messages;
	std::mutex m_messagesMutex;
	msgCallback m_msgCallback;
	CTools *m_tools;
};

}

#endif // __WS_CONNECT_84E56D497D3341A197295DF5FC501D95_H__
