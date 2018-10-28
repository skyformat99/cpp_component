#ifndef __WS_CLIENT_FFDAAA01BF7F42D2B2046B7BAD746CF7_H__
#define __WS_CLIENT_FFDAAA01BF7F42D2B2046B7BAD746CF7_H__

#include <stdint.h>
#include <string>
#include <map>
#include <mutex>
#include <list>
#include <functional>
#include <condition_variable>
#include <thread>

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>
#include <websocketpp/common/thread.hpp>
#include <websocketpp/common/memory.hpp>

#include "commuchannel/commuchannel.h"

namespace websocket
{

class CTools;
class CRequest;
class CResponse;
class CClient
{
public:
	typedef websocketpp::client<websocketpp::config::asio> client;

private:
	typedef std::function<void(const std::string &msg)> msgCallback;
	typedef std::function<void(const CRequest &request, CResponse &response, CClient *client)> ntfCallback;

public:
	explicit CClient(const std::string &url, int connTimeoutMS = 1000 * 30, bool isReconnect = true);
	virtual ~CClient();

public:
	void disconnect();
	void setMessageCallback(msgCallback f);
	void registerNtfCallback(const std::string &topic, ntfCallback f);
	void unRegisterNtfCallback(const std::string &topic);

public:
	int get(const std::string &uri
		, const std::string &version
		, const std::map<std::string, std::string> *params
		, const std::map<std::string, std::string> *headers
		, std::string *response);
	int post(const std::string &uri
		, const std::string &version
		, const std::map<std::string, std::string> *params
		, const std::map<std::string, std::string> *headers
		, const std::string &body, std::string *reponse);
	int put(const std::string &uri
		, const std::string &version
		, const std::map<std::string, std::string> *params
		, const std::map<std::string, std::string> *headers
		, const std::string &body, std::string *response);
	int del(const std::string &uri
		, const std::string &version
		, const std::map<std::string, std::string> *params
		, const std::map<std::string, std::string> *headers
		, std::string *response);
	int asyncSend(const std::string &method, const std::string &uri
		, const std::string &version
		, const std::map<std::string, std::string> *params
		, const std::map<std::string, std::string> *headers
		, const std::string &body
		, websocketpp::frame::opcode::value type = websocketpp::frame::opcode::text);
	int sendMessage(const std::string &msg);

public:
	operator bool()
	{
		return m_connFlag;
	}

private:
	void onMessage(websocketpp::connection_hdl h, client::message_ptr msg);
	void onOpen(client *c, websocketpp::connection_hdl hdl);
	void onClose(client *c, websocketpp::connection_hdl hdl);

private:
	int send(const std::string *method, const std::string *uri
		, const std::string *version
		, const std::map<std::string, std::string> *params
		, const std::map<std::string, std::string> *headers
		, const std::string *body
		, std::string *response
		, int sendTimeoutMS = 30 * 1000
		, websocketpp::frame::opcode::value type = websocketpp::frame::opcode::text);
	std::string joinRaw(const std::string *method
		, const std::string *uri
		, const std::string *version
		, const std::map<std::string, std::string> *params
		, const std::map<std::string, std::string> *headers
		, const std::string *body, std::string &raw, bool isAddMsgUuid = false);
	void joinParams(const std::string &uri, const std::map<std::string, std::string> &params, std::string &str);
	void handleSyncMessage(const std::string &messageUuid, const std::string &msg);
	void checkConnect();
	void connect();

private:
	struct CMessage
	{
		std::mutex mutex;
		std::condition_variable cv;
		std::string response;
	};

private:
	client m_client;
	std::string m_url;
	client::connection_ptr m_conn;
	std::mutex m_connMutex;
	std::condition_variable m_connCv;
	commuchannel::CChannel<bool> m_connChannel;
	bool m_connFlag;
	int m_connTimeoutMS;
    websocketpp::lib::shared_ptr<websocketpp::lib::thread> m_thread;
	std::map<std::string, CMessage*> m_messages;
	std::mutex m_messagesMutex;
	std::map<std::string, ntfCallback> m_ntfTopics;
	std::mutex m_ntfTopicsMutex;
	msgCallback m_msgCallback;
	CTools *m_tools;
	bool m_isConnect;
	bool m_reConnectIsEnd;
	std::thread m_reConnectThread;
};

}

#endif // __WS_CLIENT_FFDAAA01BF7F42D2B2046B7BAD746CF7_H__
