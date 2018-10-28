#include "ws_common/ws_response.h"

namespace websocket
{

CResponse::CResponse(const endpoint::connection_ptr &conn)
	: m_conn(const_cast<endpoint::connection_ptr&>(conn))
{
}

CResponse::~CResponse()
{
}

///////////////////////////
CMsgResponse::CMsgResponse(endpoint::connection_ptr conn, endpoint::message_ptr msg)
	: CResponse(conn)
{
}

CMsgResponse::~CMsgResponse()
{
}

void CMsgResponse::addHeader(const std::string &key, const std::string &value)
{
	m_response.append_header(key, value);
}

void CMsgResponse::setStatus(status statusCode, const std::string &statusDesc)
{
	m_response.set_status(statusCode, statusDesc);
}

void CMsgResponse::setBody(const std::string &body)
{
	m_response.set_body(body);
}

void CMsgResponse::reply()
{
	m_conn->send(m_response.raw(), websocketpp::frame::opcode::text);
}

//////////////////////////////////
CHttpResponse::CHttpResponse(const endpoint::connection_ptr &conn)
	: CResponse(conn)
{
}

CHttpResponse::~CHttpResponse()
{
}

void CHttpResponse::addHeader(const std::string &key, const std::string &value)
{
	m_conn->append_header(key, value);
}

void CHttpResponse::setStatus(status statusCode, const std::string &statusDesc)
{
	m_conn->set_status(statusCode, statusDesc);
}

void CHttpResponse::setBody(const std::string &body)
{
	m_conn->set_body(body);
}

void CHttpResponse::reply()
{
	websocketpp::lib::error_code ec;
	m_conn->send_http_response(ec);
}

}
