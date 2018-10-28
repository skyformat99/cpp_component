#include "ws_common/ws_const.h"
#include "ws_common/ws_request.h"
#include "ws_common/ws_response.h"
#include "ws_server/ws_server.h"
#include "url_parse.h"
#include "http_encode.h"
#include "communication.h"
#include <thread>

namespace websocket
{

CCommunication::CCommunication(CServer *server, int msgQueueMax)
	: m_wsServer(server)
	, m_urlParse()
	, m_msgQueueMax(msgQueueMax)
	, m_handlers()
	, m_handlerMutex()
{
}

CCommunication::~CCommunication()
{
}

void CCommunication::subscribe(const std::string &topic, CHandler *handler)
{
	if (handler == nullptr) return;
	handler->setServer(m_wsServer);
	std::lock_guard<std::mutex> lk(m_handlerMutex);
	std::string t(topic);
	size_t preIndex = t.find_first_of("/");
	if (preIndex != 0) {
		t.insert(0, "/");
	}
	size_t lastIndex = t.find_last_of("/");
	if (lastIndex == t.size() - 1) {
		t.pop_back();
	}
	m_handlers.insert(std::make_pair(t, handler));
	m_urlParse.registerUrl(t);
}

void CCommunication::unSubscribe(const std::string &topic)
{
	std::lock_guard<std::mutex> lk(m_handlerMutex);
	auto iter = m_handlers.find(topic);
	if (iter != m_handlers.end()) {
		m_handlers.erase(iter);
	}
	m_urlParse.unRegisterUrl(topic);
}

CHandler *CCommunication::findHandler(const std::string &topic)
{
	auto iter = m_handlers.find(topic);
	if (iter != m_handlers.end()) {
		return iter->second;
	}
	return nullptr;
}


///////////////////////////////////////////
CMsgCommunication::CMsgCommunication(CServer *server, int msgQueueMax)
	: CCommunication(server, msgQueueMax)
	, m_isRunBroadcast(true)
	, m_broadcastThread()
	, m_bcSubUrlParse()
	, m_httpEncode()
	, m_bcSubs()
	, m_bcSubsMutex()
	, m_msgQueue()
	, m_msgQueueMutex()
	, m_msgQueueCv()
{
}

CMsgCommunication::~CMsgCommunication()
{
	m_isRunBroadcast = false;
	m_msgQueueCv.notify_all();
	m_broadcastThread.join();
}

void CMsgCommunication::broadcastHandler()
{
	while (m_isRunBroadcast)
	{
		std::unique_lock<std::mutex> lk(m_msgQueueMutex);
		if (m_msgQueue.empty()) {
			m_msgQueueCv.wait(lk);
		}
		CMessageInfo m = m_msgQueue.front();
		m_msgQueue.pop();
		lk.unlock();
		// join msg
		std::string msg("");
		m_httpEncode.getRaw("POST", &m.topic, &m.params, &m.headers, &m.body, msg);
		// find subscribers
		std::set<websocketpp::connection_hdl, std::owner_less<websocketpp::connection_hdl>> subscribers;
		bool is = findSubscribers(m.topic, subscribers);
		if (!is) continue;
		// broadcast
		broadcast(subscribers, msg);
	}
}

void CMsgCommunication::broadcast(const std::set<websocketpp::connection_hdl, std::owner_less<websocketpp::connection_hdl>> &subscribers, const std::string &msg, websocketpp::frame::opcode::value type /* = websocketpp::frame::opcode::text */)
{
	for (auto iter = subscribers.begin(); iter != subscribers.end(); ++iter) {
		m_wsServer->getServer().send(*iter, msg, type);
	}
	m_msgQueueCv.notify_one();
}

void CMsgCommunication::pushBroadcastMessage(const std::string &topic , const std::map<std::string, std::string> *headers , const std::string *body, const std::map<std::string, std::string> *params /* = nullptr */)
{
	std::unique_lock<std::mutex> lk(m_msgQueueMutex);
	if (m_msgQueue.size() > m_msgQueueMax) {
		m_msgQueueCv.wait(lk);
	}
	lk.unlock();
	// push queue
	CMessageInfo m;
	m.topic = topic;
	if (params != nullptr) m.params = *params;
	if (headers != nullptr) m.headers = *headers;
	if (body != nullptr) m.body = *body;
	m_msgQueueMutex.lock();
	m_msgQueue.push(m);
	m_msgQueueMutex.unlock();
	m_msgQueueCv.notify_one();
}

void CMsgCommunication::startBroadcastThread()
{
	m_broadcastThread = std::thread(&CMsgCommunication::broadcastHandler, this);
}

void CMsgCommunication::subscribeBroadcast(const std::string &topic, CRequest *req)
{
	std::lock_guard<std::mutex> lk(m_bcSubsMutex);
	auto hdl = req->getHdl();
	auto iter = m_bcSubs.find(topic);
	if (iter != m_bcSubs.end()) {
		iter->second.insert(hdl);
	}
	else {
		std::set<websocketpp::connection_hdl, std::owner_less<websocketpp::connection_hdl>> hdls;
		hdls.insert(hdl);
		m_bcSubs.insert(std::make_pair(topic, hdls));
		m_bcSubUrlParse.registerUrl(topic);
	}
}

void CMsgCommunication::unSubscribeBroadcast(const std::string &topic, CRequest *req)
{
	auto hdl = req->getHdl();
	removeBroadcastSubscribe(topic, hdl);
}

void CMsgCommunication::removeBroadcastSubscribe(const std::string &topic, websocketpp::connection_hdl hdl)
{
	std::lock_guard<std::mutex> lk(m_bcSubsMutex);
	auto iter = m_bcSubs.find(topic);
	if (iter != m_bcSubs.end()) {
		auto it = iter->second.find(hdl);
		if (it != iter->second.end()) {
			iter->second.erase(it);
		}
		if (iter->second.empty()) {
			m_bcSubs.erase(iter);
			m_bcSubUrlParse.unRegisterUrl(topic);
		}
	}
}

void CMsgCommunication::removeBroadcastSubscribe(websocketpp::connection_hdl hdl)
{
	std::lock_guard<std::mutex> lk(m_bcSubsMutex);
	for (auto iter = m_bcSubs.begin(); iter != m_bcSubs.end(); ++iter) {
		auto it = iter->second.find(hdl);
		if (it != iter->second.end()) {
			iter->second.erase(it);
			if (iter->second.empty()) {
				m_bcSubUrlParse.unRegisterUrl(iter->first);
				m_bcSubs.erase(iter);
			}
			break;
		}
	}
}

bool CMsgCommunication::findSubscribers(const std::string &topic, std::set<websocketpp::connection_hdl, std::owner_less<websocketpp::connection_hdl>> &subscribers)
{
	// parse topic
	std::string url("");
	std::vector<std::string> result;
	bool is = m_bcSubUrlParse.findMatchUrl(topic, url, result);
	if (!is) return false;
	// find this topic subscribers
	auto iter = m_bcSubs.find(url);
	if (iter == m_bcSubs.end()) {
		return false;
	}
	subscribers = iter->second;
	return true;
}

void CMsgCommunication::handleMessage(const websocketpp::connection_hdl &hdl, server::message_ptr msg)
{
	auto conn = m_wsServer->getServer().get_con_from_hdl(hdl);
	CMsgRequest request(hdl, msg);
	CMsgResponse response(conn, msg);
	const std::string &messageUuid = request.header(__messageuuid__);
	if (messageUuid != "") {
		response.addHeader(__messageuuid__, messageUuid);
	}
	const std::string &path = request.topic();
	std::string url("");
	std::vector<std::string> matchResults;
	bool is = m_urlParse.findMatchUrl(path, url, matchResults);
	if (!is) return;
	request.setMatchResult(matchResults);
	CHandler *handler = findHandler(url);
	if (handler == nullptr) return;
	handler->handle(&request, &response, m_wsServer);
}

/////////////////////////////////////
CHttpCommunication::CHttpCommunication(CServer *server, int msgQueueMax)
	: CCommunication(server, msgQueueMax)
{
}

CHttpCommunication::~CHttpCommunication()
{
}

void CHttpCommunication::handleMessage(const websocketpp::connection_hdl &hdl, server::message_ptr msg)
{
	const auto &conn = m_wsServer->getServer().get_con_from_hdl(hdl);
	CHttpRequest request(conn);
	CHttpResponse response(conn);
	const std::string &path = request.topic();
	std::string url("");
	std::vector<std::string> matchResults;
	bool is = m_urlParse.findMatchUrl(path, url, matchResults);
	if (!is) return;
	request.setMatchResult(matchResults);
	CHandler *handler = findHandler(url);
	if (handler == nullptr) return;
	handler->handle(&request, &response, m_wsServer);
}

}
