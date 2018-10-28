#include "url_parse.h"
#include <exception>
#include <regex>

namespace websocket
{

CUrlParse::CUrlParse()
	: m_urls()
	, m_mutex()
{
}

CUrlParse::~CUrlParse()
{
}

void CUrlParse::registerUrl(const std::string &url)
{
	std::lock_guard<std::mutex> lk(m_mutex);
	std::string tmp = replaceAll(url, "+", "([^/]*?)");
	tmp = replaceAll(tmp, "#", "(.*?)");
	auto iter = m_urls.find(tmp);
	if (iter != m_urls.end()) {
		throw std::exception();
	}
	m_urls.insert(std::make_pair(url, tmp));
}

void CUrlParse::unRegisterUrl(const std::string &url)
{
	std::lock_guard<std::mutex> lk(m_mutex);
	auto iter = m_urls.find(url);
	if (iter != m_urls.end()) {
		m_urls.erase(iter);
	}
}

bool CUrlParse::findMatchUrl(const std::string &in, std::string &url, std::vector<std::string> &params)
{
	auto iter = m_urls.begin();
	for (; iter != m_urls.end(); ++iter) {
		std::regex r(iter->second);
		std::smatch result;
		bool b = std::regex_match(in, result, r);
		if (b) {
			size_t size = result.size();
			for (size_t i = 1; i < size; ++i) {
				params.push_back(result[i]);
			}
			break;
		}
	}
	if (iter != m_urls.end()) {
		url = iter->first;
		return true;
	}
	return false;
}

std::string CUrlParse::replaceAll(const std::string &src, const std::string &srcSub, const std::string &dstSub)
{
	std::string s(src);
	size_t index = 0;
	while (1)
	{
		index = s.find(srcSub);
		if (index == -1) break;
		s.replace(index, srcSub.size(), "");
		s.insert(index, dstSub);
	}
	return s;
}

}
