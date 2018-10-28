#include "http_encode.h"
#include <websocketpp/common/system_error.hpp>
#include <websocketpp/http/request.hpp>

namespace websocket
{

CHttpEncode::CHttpEncode()
{
}

CHttpEncode::~CHttpEncode()
{
}

void CHttpEncode::getRaw(const std::string &method , const std::string *path , const std::map<std::string, std::string> *params , const std::map<std::string, std::string> *headers , const std::string *body , std::string &raw)
{
	websocketpp::http::parser::request request;
	std::string uri("");
	if (path != nullptr && params == nullptr) {
		uri.append(*path);
	}
	else if (path != nullptr && params != nullptr) {
		joinUrl(*path, *params, uri);
	}
	request.set_uri(uri);
	request.set_method(method);
	if (headers != nullptr) {
		for (auto iter = (*headers).begin(); iter != (*headers).end(); ++iter) {
			request.append_header(iter->first, iter->second);
		}
	}
	if (body != nullptr) {
		request.set_body(*body);
	}

	raw = request.raw();
}

void CHttpEncode::joinUrl(const std::string &path, const std::map<std::string, std::string> &params, std::string &str)
{
	str.assign(path);
	size_t index = path.find_last_of('/');
	if (index != path.size() - 1) {
		str.append("/");
	}

	std::stringstream ss;
	ss << "?";
	size_t size = params.size();
	size_t i = 0;
	for (auto iter = params.begin(); iter != params.end(); ++iter, ++i) {
		ss << iter->first << "=" << iter->second;
		if (i < size - 1) {
			ss << "&";
		}
	}

	str.append(ss.str());
}

}
