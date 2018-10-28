#ifndef __HTTP_ENCODE_787323E046574BEA861E8041DBA6025A_H__
#define __HTTP_ENCODE_787323E046574BEA861E8041DBA6025A_H__

#include <map>
#include <string>

namespace websocket
{

class CHttpEncode
{
public:
	explicit CHttpEncode();
	virtual ~CHttpEncode();

public:
	void getRaw(const std::string &method
		, const std::string *path
		, const std::map<std::string, std::string> *params
		, const std::map<std::string, std::string> *headers
		, const std::string *body
		, std::string &raw);

private:
	void joinUrl(const std::string &path, const std::map<std::string, std::string> &params, std::string &str);
};

}

#endif
