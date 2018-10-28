#ifndef __URI_FILTER_H__
#define __URI_FILTER_H__
#include <vector>
#include <string>
#include <map>

namespace msf {

class IHttpHandler;

class CUriFilter
{
public:
	explicit CUriFilter();
	~CUriFilter();
	void subscribe(const std::string& uri, IHttpHandler* handler);
	void unsubscribe(const std::string& uri);
	IHttpHandler* handler(const std::string& uri);
private:
	std::vector<std::string> sections(const std::string& uri);
	void subscribe(const std::vector<std::string>& sections, IHttpHandler* handler, int32_t depth = 0);
	void unsubscribe(const std::vector<std::string>& sections, int32_t depth = 0);
	IHttpHandler* handler(const std::vector<std::string>& sections, int32_t depth = 0);
	void clean();
private:
	std::map<std::string, std::pair<CUriFilter*, IHttpHandler*>> m_filters;
};

}

#endif //__URI_FILTER_H__
