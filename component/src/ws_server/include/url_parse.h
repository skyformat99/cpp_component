#ifndef __URL_PARSE_7AF95161C7544B188B4FA9156EE4A537_H__
#define __URL_PARSE_7AF95161C7544B188B4FA9156EE4A537_H__

#include <string>
#include <map>
#include <mutex>
#include <vector>

namespace websocket
{

class CUrlParse
{
public:
	explicit CUrlParse();
	virtual ~CUrlParse();

public:
	void registerUrl(const std::string &url);
	void unRegisterUrl(const std::string &url);
	bool findMatchUrl(const std::string &in, std::string &url, std::vector<std::string> &params);

private:
	std::string replaceAll(const std::string &src, const std::string &srcSub, const std::string &dstSub);

private:
	std::map<std::string, std::string> m_urls;
	std::mutex m_mutex;
};

}

#endif // __URL_PARSE_7AF95161C7544B188B4FA9156EE4A537_H__
