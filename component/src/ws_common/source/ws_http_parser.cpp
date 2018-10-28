#include "ws_common/ws_http_parser.h"
#include <vector>

namespace websocket
{

CHttpParser::CHttpParser()
	: m_stringTools()
{
}

CHttpParser::~CHttpParser()
{
}

void CHttpParser::urlParse(const std::string &url, std::string &topic , std::map<std::string, std::string> &params)
{
	topic.assign(url);
	size_t index = url.find_first_of("?");
	if (index == -1) {
		return;
	}
	else {
		topic.replace(index, -1, "");
	}
	const std::string &paramString = url.substr(index + 1, -1);
	std::vector<std::string> kvs;
	m_stringTools.split(paramString, "&", kvs);
	if (kvs.empty()) return;
	for (auto iter = kvs.begin(); iter != kvs.end(); ++iter) {
		std::vector<std::string> kv;
		m_stringTools.split(*iter, "=", kv);
		if (kv.empty() || kv.size() != 2) continue;
		params.insert(std::make_pair(kv[0], kv[1]));
	}
}

void CHttpParser::headerParse(const std::string &line , std::string &k, std::string &v)
{
	std::vector<std::string> kvs;
	m_stringTools.split(line, ":", kvs);
	if (kvs.empty() || kvs.size() != 2) return;
	k.assign(kvs[0]);
	v.assign(kvs[1]);
	m_stringTools.trim(k);
	m_stringTools.trim(v);
}

}
