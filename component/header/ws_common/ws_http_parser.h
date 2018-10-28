#ifndef __WS_HTTP_PARSER_72AC1BD2567240D48D8EFE599E62622C_H__
#define __WS_HTTP_PARSER_72AC1BD2567240D48D8EFE599E62622C_H__

#include <string>
#include <map>
#include "ws_common/ws_string_tools.h"
#include "ws_http_parser.h"

namespace websocket
{

class CHttpParser
{
public:
	explicit CHttpParser();
	virtual ~CHttpParser();

public:
	void urlParse(const std::string &url, std::string &topic
		, std::map<std::string, std::string> &params);
	void headerParse(const std::string &line
		, std::string &k, std::string &v);

private:
	CStringTools m_stringTools;
};

}

#endif // __WS_HTTP_PARSER_72AC1BD2567240D48D8EFE599E62622C_H__
