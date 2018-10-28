#include <sstream>
#include <thread>
#include <iostream>
#include "ws_client/ws_client.h"
#include "tools.h"
#include "ws_common/ws_const.h"
#include "ws_common/ws_msg_parser.h"
#include "ws_common/ws_request.h"
#include "ws_common/ws_response.h"

namespace websocket
{

CClient::CClient(const std::string &url, int connTimeoutMS /* = 1000 * 30 */, bool isReconnect /* = true */)
	: m_client()
	, m_url(url)
	, m_conn()
	, m_connMutex()
	, m_connCv()
	, m_connChannel()
	, m_connFlag(false)
	, m_connTimeoutMS(connTimeoutMS)
	, m_thread()
	, m_messages()
	, m_ntfTopics()
	, m_ntfTopicsMutex()
	, m_messagesMutex()
	, m_msgCallback()
	, m_tools(new CTools())
	, m_isConnect(false)
	, m_reConnectIsEnd(false)
	, m_reConnectThread()
{
	// init
	m_client.clear_access_channels(websocketpp::log::alevel::all);
	m_client.clear_error_channels(websocketpp::log::elevel::all);
	m_client.init_asio();
	m_client.start_perpetual();
	// start loop thread
	m_thread = websocketpp::lib::make_shared<websocketpp::lib::thread>(&client::run, &m_client);
	connect();
	if (isReconnect) {
		checkConnect();
	}
}

CClient::~CClient()
{
	disconnect();
	m_thread->join();
	if (m_tools != nullptr) {
		delete m_tools;
		m_tools = nullptr;
	}
	m_client.stop_perpetual();
}

void CClient::disconnect()
{
	m_reConnectIsEnd = true;
	m_reConnectThread.join();
	websocketpp::lib::error_code ec;
	m_client.close(m_conn->get_handle(), websocketpp::close::status::going_away, "", ec);
}

void CClient::checkConnect()
{
	m_reConnectThread = std::thread([this] {
		while (!m_reConnectIsEnd) {
			if (m_isConnect) {
				std::this_thread::sleep_for(std::chrono::milliseconds(1000));
				continue;
			}
			connect();
		}
	});
}

void CClient::connect()
{
	websocketpp::lib::error_code ec;
	m_conn = m_client.get_connection(m_url, ec);
	if (ec) {
		return;
	}
	// register callback
	m_conn->set_open_handler(websocketpp::lib::bind(
		&CClient::onOpen,
		this,
		&m_client,
		websocketpp::lib::placeholders::_1
	));
	m_conn->set_close_handler(websocketpp::lib::bind(
		&CClient::onClose, this, &m_client, websocketpp::lib::placeholders::_1
	));
	m_conn->set_message_handler(websocketpp::lib::bind(
		&CClient::onMessage,
		this,
		websocketpp::lib::placeholders::_1,
		websocketpp::lib::placeholders::_2
	));

	auto ptr = m_client.connect(m_conn);
	if (!ptr) {
		return;
	}

	// wait connect
	bool result = false;
	bool ret = m_connChannel.read(result, m_connTimeoutMS);
	if (ret) {
		m_connFlag = true;
		m_isConnect = true;
	}
#if 0
	std::unique_lock<std::mutex> lk(m_connMutex);
	auto ret = m_connCv.wait_for(lk, std::chrono::milliseconds(m_connTimeoutMS));
	if (ret == std::cv_status::no_timeout) {
		m_connFlag = true;
		m_isConnect = true;
	}
	lk.unlock();
#endif
}

void CClient::setMessageCallback(msgCallback f)
{
	m_msgCallback = f;
}

void CClient::registerNtfCallback(const std::string &topic, ntfCallback f)
{
	std::lock_guard<std::mutex> lk(m_ntfTopicsMutex);
	m_ntfTopics.insert(std::make_pair(topic, f));
}

void CClient::unRegisterNtfCallback(const std::string &topic)
{
	std::lock_guard<std::mutex> lk(m_ntfTopicsMutex);
	m_ntfTopics.erase(topic);
}

void CClient::onOpen(client *c, websocketpp::connection_hdl hdl)
{
	m_connChannel.write(true);
}

void CClient::onClose(client *c, websocketpp::connection_hdl hdl)
{
	m_connFlag = false;
	m_isConnect = false;
}

void CClient::onMessage(websocketpp::connection_hdl hdl, client::message_ptr msg)
{
	std::thread([this, hdl, msg] {
		if (m_msgCallback) {
			m_msgCallback(msg->get_raw_payload());
		}
		CMsgRequest request(hdl, msg);
		auto conn = m_client.get_con_from_hdl(hdl);
		CMsgResponse response(conn, msg);
		auto iter = m_ntfTopics.find(request.topic());
		if (iter != m_ntfTopics.end()) {
			if (iter->second) {
				(iter->second)(request, response, this);
			}
		}
	}).detach();
	// handle sync send
	CMsgRequest request(hdl, msg);
	const std::string &messageUuid = request.header(__messageuuid__);
	if (messageUuid != "") {
		handleSyncMessage(messageUuid, request.body());
	}
}

void CClient::handleSyncMessage(const std::string &messageUuid, const std::string &msg)
{
	auto iter = m_messages.find(messageUuid);
	if (iter == m_messages.end()) return;
	iter->second->cv.notify_one();
	iter->second->response = msg;
}

int CClient::get(const std::string &uri , const std::string &version , const std::map<std::string, std::string> *params , const std::map<std::string, std::string> *headers , std::string *response)
{
	return send(&websocket::method_get, &uri, &version, params, headers, nullptr, response);
}

int CClient::post(const std::string &uri , const std::string &version , const std::map<std::string, std::string> *params , const std::map<std::string, std::string> *headers , const std::string &body, std::string *reponse)
{
	return send(&websocket::method_post, &uri, &version, params, headers, &body, reponse);
}

int CClient::put(const std::string &uri , const std::string &version , const std::map<std::string, std::string> *params , const std::map<std::string, std::string> *headers , const std::string &body, std::string *response)
{
	return send(&websocket::method_put, &uri, &version, params, headers, &body, response);
}

int CClient::del(const std::string &uri , const std::string &version , const std::map<std::string, std::string> *params , const std::map<std::string, std::string> *headers , std::string *response)
{
	return send(&websocket::method_delete, &uri, &version, params, headers, nullptr, response);
}

int CClient::asyncSend(const std::string &method, const std::string &uri , const std::string &version , const std::map<std::string, std::string> *params , const std::map<std::string, std::string> *headers , const std::string &body , websocketpp::frame::opcode::value type /* = websocketpp::frame::opcode::text */)
{
	if (!m_conn) return -1;

	std::string raw("");
	joinRaw(&method, &uri, &version, params, headers, &body, raw, true);

	websocketpp::lib::error_code ec;
	m_client.send(m_conn->get_handle(), raw, type, ec);
	if (ec) {
		return -1;
	}

	return 0;
}

int CClient::sendMessage(const std::string &msg)
{
	websocketpp::lib::error_code ec;
	m_client.send(m_conn->get_handle(), msg, websocketpp::frame::opcode::text, ec);
	if (ec) {
		return -1;
	}

	return 0;
}

int CClient::send(const std::string *method, const std::string *uri , const std::string *version , const std::map<std::string, std::string> *params , const std::map<std::string, std::string> *headers , const std::string *body , std::string *response , int sendTimeoutMS /* = 30 * 1000  */, websocketpp::frame::opcode::value type /* = websocketpp::frame::opcode::text */)
{
	if (!m_conn) return -1;

	std::string raw("");
	const std::string &msgUuid = joinRaw(method, uri, version, params, headers, body, raw, true);

	websocketpp::lib::error_code ec;
	m_client.send(m_conn->get_handle(), raw, type, ec);
	if (ec) {
		return -1;
	}

	CMessage message;
	m_messagesMutex.lock();
	m_messages.insert(std::make_pair(msgUuid, &message));
	m_messagesMutex.unlock();
	std::unique_lock<std::mutex> lk(message.mutex);
	message.cv.wait_for(lk, std::chrono::milliseconds(sendTimeoutMS));
	m_messagesMutex.lock();
	m_messages.erase(msgUuid);
	m_messagesMutex.unlock();
	if (response != nullptr) {
		*response = message.response;
	}

	return 0;
}

std::string CClient::joinRaw(const std::string *method , const std::string *uri , const std::string *version , const std::map<std::string, std::string> *params , const std::map<std::string, std::string> *headers , const std::string *body, std::string &raw, bool isAddMsgUuid /* = false */)
{
	websocketpp::http::parser::request request;

	std::string u("");
	if (uri != nullptr && params != nullptr) {
		// join uri
		joinParams(*uri, *params, u);
		request.set_uri(u);
	}
	if (uri != nullptr && params == nullptr) {
		request.set_uri(*uri);
	}
	if (method != nullptr) {
		request.set_method(*method);
	}
	if (version != nullptr) {
		request.set_version(*version);
	}
	if (headers != nullptr) {
		for (auto iter = (*headers).begin(); iter != (*headers).end(); ++iter) {
			request.append_header(iter->first, iter->second);
		}
	}
	if (body != nullptr) {
		request.set_body(*body);
	}

	std::string msgUuid("");
	if (isAddMsgUuid) {
		// gen uuid
		msgUuid = m_tools->generateUUID();
		request.append_header(__messageuuid__, msgUuid);
	}

	raw = request.raw();

	return msgUuid;
}

void CClient::joinParams(const std::string &uri, const std::map<std::string, std::string> &params, std::string &str)
{
	str.assign(uri);
	if (params.empty()) return;
	std::stringstream ss;
	ss << "?";
	size_t size = params.size();
	size_t i = 0;
	for (auto iter = params.begin(); iter != params.end(); ++iter, ++i) {
		ss << iter->first << "=" << iter->second;
		if (i < size - 1) {
			ss << "&";
		}
	}
	str.append(ss.str());
}

}
