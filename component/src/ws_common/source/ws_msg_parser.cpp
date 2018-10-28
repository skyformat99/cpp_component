#include "ws_common/ws_msg_parser.h"

namespace websocket
{

CMsgParser::CMsgParser()
	: m_method()
	, m_topic()
	, m_version()
	, m_params()
	, m_headers()
	, m_body()
{
}

CMsgParser::~CMsgParser()
{
}

const std::string &CMsgParser::method() const
{
	return m_method;
}

const std::string &CMsgParser::topic() const
{
	return m_topic;
}

const std::string &CMsgParser::version() const
{
	return m_version;
}

std::string CMsgParser::param(const std::string &key) const
{
	auto iter = m_params.find(key);
	if (iter == m_params.end()) {
		return "";
	}
	return iter->second;
}

std::string CMsgParser::header(const std::string &key) const
{
	auto iter = m_headers.find(key);
	if (iter == m_headers.end()) {
		return "";
	}
	return iter->second;
}

const std::string &CMsgParser::body() const
{
	return m_body;
}


/////////////////////////////////////////
CCustomMsgParser::CCustomMsgParser()
	: CMsgParser()
{
}

CCustomMsgParser::~CCustomMsgParser()
{
}

void CCustomMsgParser::parse(const std::string &message)
{
	std::vector<std::string> lines;
	m_stringTools.split(message, "\r\n", lines);
	if (lines.empty()) return;
	const std::string &base = lines[0];
	// parse base info
	std::vector<std::string> bases;
	m_stringTools.split(base, " ", bases);
	if (bases.empty() || bases.size() != 3) return;
	m_method = bases[0];
	m_httpParser.urlParse(bases[1], m_topic, m_params);
	m_version = bases[2];
	// parse header
	auto iter = lines.begin() + 1;
	while (iter != lines.end() && *iter != "") {
		std::string k("");
		std::string v("");
		m_httpParser.headerParse(*iter, k, v);
		m_headers.insert(std::make_pair(k, v));
		++iter;
	}
	if (iter == lines.end()) return;
	// to body line
	++iter;
	for (; iter != lines.end(); ++iter) {
		m_body.append(*iter);
		if (iter != lines.end() - 1) {
			m_body.append("\r\n");
		}
	}
}


}
