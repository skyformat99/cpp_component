#include "ws_common/ws_request.h"
#include "ws_common/ws_string_tools.h"
#include "ws_common/ws_msg_parser.h"

namespace websocket
{

CRequest::CRequest(websocketpp::connection_hdl hdl)
	: m_hdl(hdl)
	, m_matchResult()
{
}

CRequest::~CRequest()
{
}

const std::vector<std::string> &CRequest::matchResult()
{
	return m_matchResult;
}


///////////////////////////
CMsgRequest::CMsgRequest(websocketpp::connection_hdl hdl, endpoint::message_ptr msg)
	: CRequest(hdl)
	, m_msg(msg)
	, m_msgParser(new CCustomMsgParser())
{
	m_msgParser->parse(m_msg->get_raw_payload());
}

CMsgRequest::~CMsgRequest()
{
	if (m_msgParser != nullptr) {
		delete m_msgParser;
		m_msgParser = nullptr;
	}
}

std::string CMsgRequest::method()
{
	return m_msgParser->method();
}

std::string CMsgRequest::topic()
{
	return m_msgParser->topic();
}

std::string CMsgRequest::header(const std::string &key)
{
	return m_msgParser->header(key);
}

std::string CMsgRequest::param(const std::string &key)
{
	return m_msgParser->param(key);
}

std::string CMsgRequest::body()
{
	return m_msgParser->body();
}

//////////////////////////////////////
CHttpRequest::CHttpRequest(endpoint::connection_ptr conn)
	: CRequest(conn->get_handle())
	, m_conn(conn)
	, m_httpParser()
	, m_uri("")
	, m_params()
{
	m_httpParser.urlParse(m_conn->get_request().get_uri(), m_uri, m_params);
}

CHttpRequest::~CHttpRequest()
{
}

std::string CHttpRequest::method()
{
	return m_conn->get_request().get_method();
}

std::string CHttpRequest::topic()
{
	return m_uri;
}

std::string CHttpRequest::header(const std::string &key)
{
	return m_conn->get_request_header(key);
}

std::string CHttpRequest::param(const std::string &key)
{
	auto iter = m_params.find(key);
	if (iter == m_params.end()) {
		return "";
	}
	return iter->second;
}

std::string CHttpRequest::body()
{
	return m_conn->get_request_body();
}

}
