#ifndef __WS_STRING_TOOLS_A18D1C6A63BD44FC86B5BD65F869B55B_H__
#define __WS_STRING_TOOLS_A18D1C6A63BD44FC86B5BD65F869B55B_H__

#include <vector>
#include <string>

namespace websocket
{

class CStringTools
{
public:
	explicit CStringTools();
	virtual ~CStringTools();

public:
	void split(const std::string &s, const std::string &pattern, std::vector<std::string> &subs);
	void trim(std::string &s);
};

}

#endif // __WS_STRINGS_TOOLS_A18D1C6A63BD44FC86B5BD65F869B55B_H__
