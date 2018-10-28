#include "ws_server/ws_server.h"
#include "url_parse.h"
#include "http_encode.h"
#include "communication.h"
#include <sstream>
#include <thread>
#include <vector>

namespace websocket
{

CServer::CServer(int msgQueueMax /* = 100 */)
	: m_server()
	, m_connSet()
	, m_connMutex()
	, m_msgCommunication(new CMsgCommunication(this, msgQueueMax))
	, m_httpCommunication(new CHttpCommunication(this, msgQueueMax))
{
	m_server.clear_access_channels(websocketpp::log::alevel::all);
	m_server.set_access_channels(websocketpp::log::alevel::access_core);
	m_server.set_access_channels(websocketpp::log::alevel::app);
	m_server.init_asio();
	m_server.set_open_handler(bind(&CServer::onOpen, this, websocketpp::lib::placeholders::_1));
	m_server.set_close_handler(bind(&CServer::onClose, this, websocketpp::lib::placeholders::_1));
	m_server.set_message_handler(bind(&CServer::onMessage, this, websocketpp::lib::placeholders::_1, websocketpp::lib::placeholders::_2));
	m_server.set_http_handler(bind(&CServer::onHttp, this, websocketpp::lib::placeholders::_1));
}

CServer::~CServer()
{
	if (m_httpCommunication != nullptr) {
		delete m_httpCommunication;
		m_httpCommunication = nullptr;
	}
	if (m_msgCommunication != nullptr) {
		delete m_msgCommunication;
		m_msgCommunication = nullptr;
	}
}

void CServer::run(uint16_t port)
{
	m_server.listen(port);
	m_server.start_accept();
	m_msgCommunication->startBroadcastThread();

#if 0
	typedef websocketpp::lib::shared_ptr<websocketpp::lib::thread> thread_ptr;
	std::vector<thread_ptr> ts;
	for (size_t i = 0; i < 100; i++) {
		ts.push_back(websocketpp::lib::make_shared<websocketpp::lib::thread>(&server::run, &m_server));
	}

	for (size_t i = 0; i < 100; i++) {
		ts[i]->join();
	}
#endif

	m_server.run();
}

void CServer::pushBroadcastMessage(const std::string &topic , const std::map<std::string, std::string> *headers , const std::string *body, const std::map<std::string, std::string> *params /* = nullptr */)
{
	m_msgCommunication->pushBroadcastMessage(topic, headers, body, params);
}

void CServer::subscribe(const std::string &topic, CHandler *handler, communication_type type)
{
	auto commu = communication(type);
	if (commu == nullptr) return;
	commu->subscribe(topic, handler);
}

void CServer::unSubscribe(const std::string &topic, communication_type type)
{
	auto commu = communication(type);
	if (commu == nullptr) return;
	commu->unSubscribe(topic);
}

void CServer::subscribeBroadcast(const std::string &topic, CRequest *req)
{
	m_msgCommunication->subscribeBroadcast(topic, req);
}

void CServer::unSubscribeBroadcast(const std::string &topic, CRequest *req)
{
	m_msgCommunication->unSubscribeBroadcast(topic, req);
}

void CServer::onOpen(websocketpp::connection_hdl hdl)
{
	addConnect(hdl);
}

void CServer::onClose(websocketpp::connection_hdl hdl)
{
	removeConnect(hdl);
	m_msgCommunication->removeBroadcastSubscribe(hdl);
}

void CServer::onMessage(websocketpp::connection_hdl hdl, server::message_ptr msg)
{
	std::thread([this, hdl, msg] {
		m_msgCommunication->handleMessage(hdl, msg);
	}).detach();
}

void CServer::onHttp(websocketpp::connection_hdl hdl)
{
	std::thread([this, &hdl] {
		m_httpCommunication->handleMessage(hdl, nullptr);
	}).join();
}

void CServer::addConnect(websocketpp::connection_hdl hdl)
{
	std::lock_guard<std::mutex> lk(m_connMutex);
	m_connSet.insert(hdl);
}

void CServer::removeConnect(websocketpp::connection_hdl hdl)
{
	std::lock_guard<std::mutex> lk(m_connMutex);
	auto iter = m_connSet.find(hdl);
	if (iter != m_connSet.end()) {
		m_connSet.erase(iter);
	}
}

CCommunication *CServer::communication(communication_type type)
{
	switch (type)
	{
	case communication_type::http:
		return m_httpCommunication;
		break;
	case communication_type::ws:
		return m_msgCommunication;
		break;
	}
	return nullptr;
}

}
