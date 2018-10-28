#ifndef __WS_RESPONSE_00B53667DCD845628FD4F73796F3BCCE_H__
#define __WS_RESPONSE_00B53667DCD845628FD4F73796F3BCCE_H__

#include <string>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/common/connection_hdl.hpp>
#include <websocketpp/server.hpp>

namespace websocket
{

class CResponse
{
public:
	typedef websocketpp::http::status_code::value status;

protected:
	typedef websocketpp::endpoint<websocketpp::connection<websocketpp::config::asio>, websocketpp::config::asio> endpoint;

public:
	explicit CResponse(const endpoint::connection_ptr &conn);
	virtual ~CResponse();

public:
	virtual void addHeader(const std::string &key, const std::string &value) = 0;
	virtual void setStatus(status statusCode, const std::string &statusDesc) = 0;
	virtual void setBody(const std::string &body) = 0;
	virtual void reply() = 0;

protected:
	endpoint::connection_ptr &m_conn;
};

class CMsgResponse : public CResponse
{
public:
	explicit CMsgResponse(endpoint::connection_ptr conn, endpoint::message_ptr msg);
	virtual ~CMsgResponse();

public:
	virtual void addHeader(const std::string &key, const std::string &value);
	virtual void setStatus(status statusCode, const std::string &statusDesc);
	virtual void setBody(const std::string &body);
	virtual void reply();

private:
	endpoint::message_ptr m_msg;
	websocketpp::http::parser::response m_response;
};

class CHttpResponse : public CResponse
{
public:
	explicit CHttpResponse(const endpoint::connection_ptr &conn);
	virtual ~CHttpResponse();

public:
	virtual void addHeader(const std::string &key, const std::string &value);
	virtual void setStatus(status statusCode, const std::string &statusDesc);
	virtual void setBody(const std::string &body);
	virtual void reply();
};

}

#endif // __WS_RESPONSE_00B53667DCD845628FD4F73796F3BCCE_H__
