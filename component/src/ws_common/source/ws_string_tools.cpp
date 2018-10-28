#include "ws_common/ws_string_tools.h"

namespace websocket
{

CStringTools::CStringTools()
{
}

CStringTools::~CStringTools()
{
}

void CStringTools::split(const std::string &s, const std::string &pattern, std::vector<std::string> &subs)
{
	std::string str(s);
	subs.clear();
	std::string::size_type pos;
	str += pattern;
	size_t size = str.size();

	for (size_t i = 0; i < size; i++)
	{
		pos = str.find(pattern, i);
		if (pos < size)
		{
			std::string s = str.substr(i, pos - i);
			subs.push_back(s);
			i = pos + pattern.size() - 1;
		}
	}
}

void CStringTools::trim(std::string &s)
{
	if (!s.empty())
	{
		s.erase(0, s.find_first_not_of(" "));
		s.erase(s.find_last_not_of(" ") + 1);
	}
}

}
