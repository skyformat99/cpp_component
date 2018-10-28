#ifndef __MSG_PARSER_60A2AF530DA24651B533C73C125EEFB9_H__
#define __MSG_PARSER_60A2AF530DA24651B533C73C125EEFB9_H__

#include <string>
#include <vector>
#include <map>
#include "ws_http_parser.h"
#include "ws_string_tools.h"

namespace websocket
{

class CMsgParser
{
public:
	explicit CMsgParser();
	virtual ~CMsgParser();

public:
	const std::string &method() const;
	const std::string &topic() const;
	const std::string &version() const;
	std::string param(const std::string &key) const;
	std::string header(const std::string &key) const;
	const std::string &body() const;

public:
	virtual void parse(const std::string &message) = 0;

protected:
	std::string m_method;
	std::string m_topic;
	std::string m_version;
	std::map<std::string, std::string> m_params;
	std::map<std::string, std::string> m_headers;
	std::string m_body;
};

class CCustomMsgParser : public CMsgParser
{
public:
	explicit CCustomMsgParser();
	virtual ~CCustomMsgParser();

public:
	virtual void parse(const std::string &message);

private:
	CStringTools m_stringTools;
	CHttpParser m_httpParser;
};

}

#endif // __MSG_PARSER_60A2AF530DA24651B533C73C125EEFB9_H__
