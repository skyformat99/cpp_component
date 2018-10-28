#ifndef __WS_REQUEST_BCDC792F9A1C4CD0BAC7BA2AEDAF8781_H__
#define __WS_REQUEST_BCDC792F9A1C4CD0BAC7BA2AEDAF8781_H__

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/common/connection_hdl.hpp>
#include <websocketpp/server.hpp>
#include <vector>
#include <string>
#include <map>
#include "ws_common/ws_http_parser.h"

namespace websocket
{

class CStringTools;
class CMsgParser;
class CServer;
class CRequest
{
protected:
	typedef websocketpp::endpoint<websocketpp::connection<websocketpp::config::asio>, websocketpp::config::asio> endpoint;

public:
	explicit CRequest(websocketpp::connection_hdl hdl);
	virtual ~CRequest();

public:
	virtual std::string method() = 0;
	virtual std::string topic() = 0;
	virtual std::string header(const std::string &key) = 0;
	virtual std::string param(const std::string &key) = 0;
	virtual std::string body() = 0;
	const std::vector<std::string> &matchResult();

	friend class CServer;
	friend class CCommunication;
	friend class CMsgCommunication;
	friend class CHttpCommunication;
private:
	void setMatchResult(const std::vector<std::string> &results) { m_matchResult = results; }
	websocketpp::connection_hdl getHdl() { return m_hdl; }

private:
	websocketpp::connection_hdl m_hdl;
	std::vector<std::string> m_matchResult;
};

class CMsgRequest : public CRequest
{
public:
	explicit CMsgRequest(websocketpp::connection_hdl hdl, endpoint::message_ptr msg);
	virtual ~CMsgRequest();

public:
	virtual std::string method();
	virtual std::string topic();
	virtual std::string header(const std::string &key);
	virtual std::string param(const std::string &key);
	virtual std::string body();

private:
	endpoint::message_ptr m_msg;
	CMsgParser *m_msgParser;
};

class CHttpRequest : public CRequest
{
public:
	explicit CHttpRequest(endpoint::connection_ptr conn);
	virtual ~CHttpRequest();

public:
	virtual std::string method();
	virtual std::string topic();
	virtual std::string header(const std::string &key);
	virtual std::string param(const std::string &key);
	virtual std::string body();

private:
	endpoint::connection_ptr m_conn;
	CHttpParser m_httpParser;
	std::string m_uri;
	std::map<std::string, std::string> m_params;
};

}

#endif // __WS_REQUEST_BCDC792F9A1C4CD0BAC7BA2AEDAF8781_H__
