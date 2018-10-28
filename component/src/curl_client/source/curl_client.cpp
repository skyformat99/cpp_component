#include <string.h>
#include <fstream>
#include "randtool/uuid.h"
#include "mediatool/image.h"
#include "ospathtool/ospathtool.h"
#include "stringtool/stringtool.h"
#include "json/json.h"
#include "curl/curl.h"
#include "curl_client.h"
#include "base/baseerror.h"
#include <sstream>
#include "curl_connect.h"
#include "curl_conn_mgr.h"

namespace http
{

CCurlConnMgr *CCurlHttpClient::m_mgr = nullptr;
int CCurlHttpClient::init(int max /* = 500  */, const char *cainfoPath /* = nullptr  */, const char *sslcertPath /* = nullptr  */, const char *keypassword /* = nullptr */)
{
	if (m_mgr == nullptr) {
		m_mgr = new CCurlConnMgr(cainfoPath, sslcertPath, keypassword, max);
		return COMERR_OK;
	}
	return COMERR_ALREADY_INIT;
}

CCurlHttpClient::CCurlHttpClient()
	: connect_timeout(10 * 1000), socket_timeout(60 * 1000)
{
}

CCurlHttpClient::~CCurlHttpClient()
{
}

void CCurlHttpClient::setConnectTimeout(int connect_timeout)
{
	this->connect_timeout = connect_timeout;
}

void CCurlHttpClient::setSocketTimeout(int socket_timeout)
{
	this->socket_timeout = socket_timeout;
}

int CCurlHttpClient::get(
		std::string url,
		std::map<std::string, std::string> const * params,
		std::map<std::string, std::string> const * headers,
		std::string * response, int *resCode, bool isRedir
		, int reSendTimeoutMS) const
{
	if (m_mgr == nullptr) return COMERR_NOT_INIT;
	CCurlConnect *conn = m_mgr->connect(this->connect_timeout, this->socket_timeout);
	if (conn == nullptr) return COMERR_NULL_POINTER;
	return conn->send(url, params, nullptr, 0, headers, response, "GET", isRedir, resCode, nullptr, nullptr, nullptr, reSendTimeoutMS);
}

int CCurlHttpClient::get(std::string url , std::map<std::string, std::string> const * params , std::map<std::string, std::string> const * headers , std::string * response, std::map<std::string, std::string> &resHeaders , bool isRedir /* = false  */, int reSendTimeoutMS /* = -1 */) const
{
	if (m_mgr == nullptr) return COMERR_NOT_INIT;
	CCurlConnect *conn = m_mgr->connect(this->connect_timeout, this->socket_timeout);
	if (conn == nullptr) return COMERR_NULL_POINTER;
	return conn->send(url, params, nullptr, 0, headers, response, "GET", isRedir, nullptr, &resHeaders, nullptr, nullptr, reSendTimeoutMS);
}

int CCurlHttpClient::post(
		std::string url,
		std::map<std::string, std::string> const * params,
		const std::string & body,
		std::map<std::string, std::string> const * headers,
		std::string * response, int *resCode, bool isRedir
		, int reSendTimeoutMS) const
{
	if (m_mgr == nullptr) return COMERR_NOT_INIT;
	CCurlConnect *conn = m_mgr->connect(this->connect_timeout, this->socket_timeout);
	if (conn == nullptr) return COMERR_NULL_POINTER;
	return conn->send(url, params, body.c_str(), body.size(), headers, response, "POST", isRedir, resCode, nullptr, nullptr, nullptr, reSendTimeoutMS);
}

int CCurlHttpClient::post(std::string url, std::map<std::string, std::string> const * params, const std::string & body, std::map<std::string, std::string> const * headers, std::string * response, std::map<std::string, std::string> &resHeaders, int *resCode /* = nullptr */, bool isRedir /* = false */, int reSendTimeoutMS) const
{
	if (m_mgr == nullptr) return COMERR_NOT_INIT;
	CCurlConnect *conn = m_mgr->connect(this->connect_timeout, this->socket_timeout);
	if (conn == nullptr) return COMERR_NULL_POINTER;
	return conn->send(url, params, body.c_str(), body.size(), headers, response, "POST", isRedir, resCode, &resHeaders, nullptr, nullptr, reSendTimeoutMS);
}

int CCurlHttpClient::put(
	std::string url,
	std::map<std::string, std::string> const * params,
	const std::string &body,
	std::map<std::string, std::string> const * headers,
	std::string * response, int *resCode, bool isRedir
	, int reSendTimeoutMS) const
{
	if (m_mgr == nullptr) return COMERR_NOT_INIT;
	CCurlConnect *conn = m_mgr->connect(this->connect_timeout, this->socket_timeout);
	if (conn == nullptr) return COMERR_NULL_POINTER;
	return conn->send(url, params, body.c_str(), body.size(), headers, response, "PUT", isRedir, resCode, nullptr, nullptr, nullptr, reSendTimeoutMS);
}

int CCurlHttpClient::patch(
	std::string url,
	std::map<std::string, std::string> const * params,
	const std::string &body,
	std::map<std::string, std::string> const * headers,
	std::string * response, int *resCode, bool isRedir
	, int reSendTimeoutMS) const
{
	if (m_mgr == nullptr) return COMERR_NOT_INIT;
	CCurlConnect *conn = m_mgr->connect(this->connect_timeout, this->socket_timeout);
	if (conn == nullptr) return COMERR_NULL_POINTER;
	return conn->send(url, params, body.c_str(), body.size(), headers, response, "PATCH", isRedir, resCode, nullptr, nullptr, nullptr, reSendTimeoutMS);
}

int CCurlHttpClient::del(
	std::string url,
	std::map<std::string, std::string> const * params,
	std::map<std::string, std::string> const * headers,
	std::string * response, int *resCode, bool isRedir
	, int reSendTimeoutMS) const
{
	if (m_mgr == nullptr) return COMERR_NOT_INIT;
	CCurlConnect *conn = m_mgr->connect(this->connect_timeout, this->socket_timeout);
	if (conn == nullptr) return COMERR_NULL_POINTER;
	return conn->send(url, params, nullptr, 0, headers, response, "DELETE", isRedir, resCode, nullptr, nullptr, nullptr, reSendTimeoutMS);
}

int CCurlHttpClient::head(
	std::string url,
	std::map<std::string, std::string> const * params,
	std::map<std::string, std::string> const * headers,
	std::string * response, int *resCode, bool isRedir
	, int reSendTimeoutMS) const
{
	if (m_mgr == nullptr) return COMERR_NOT_INIT;
	CCurlConnect *conn = m_mgr->connect(this->connect_timeout, this->socket_timeout);
	if (conn == nullptr) return COMERR_NULL_POINTER;
	return conn->send(url, params, nullptr, 0, headers, response, "HEAD", isRedir, resCode, nullptr, nullptr, nullptr, reSendTimeoutMS);
}

int CCurlHttpClient::deleted(
	std::string url,
	std::map<std::string, std::string> const * params,
	std::map<std::string, std::string> const * headers,
	std::string * response, int *resCode, bool isRedir
	, int reSendTimeoutMS) const
{
	if (m_mgr == nullptr) return COMERR_NOT_INIT;
	CCurlConnect *conn = m_mgr->connect(this->connect_timeout, this->socket_timeout);
	if (conn == nullptr) return COMERR_NULL_POINTER;
	return conn->send(url, params, nullptr, 0, headers, response, "DELETED", isRedir, resCode, nullptr, nullptr, nullptr, reSendTimeoutMS);
}

int CCurlHttpClient::updated(
	std::string url,
	std::map<std::string, std::string> const * params,
	std::map<std::string, std::string> const * headers,
	std::string * response, int *resCode, bool isRedir
	, int reSendTimeoutMS) const
{
	if (m_mgr == nullptr) return COMERR_NOT_INIT;
	CCurlConnect *conn = m_mgr->connect(this->connect_timeout, this->socket_timeout);
	if (conn == nullptr) return COMERR_NULL_POINTER;
	return conn->send(url, params, nullptr, 0, headers, response, "UPDATED", isRedir, resCode, nullptr, nullptr, nullptr, reSendTimeoutMS);
}

int CCurlHttpClient::post(
		std::string url,
		std::map<std::string, std::string> const * params,
		const char *body, int len,
		std::map<std::string, std::string> const * headers,
		std::string * response, int *resCode, bool isRedir
		, int reSendTimeoutMS) const
{
	if (m_mgr == nullptr) return COMERR_NOT_INIT;
	CCurlConnect *conn = m_mgr->connect(this->connect_timeout, this->socket_timeout);
	if (conn == nullptr) return COMERR_NULL_POINTER;
	return conn->send(url, params, body, len, headers, response, "POST", isRedir, resCode, nullptr, nullptr, nullptr, reSendTimeoutMS);
}

int CCurlHttpClient::post(std::string url, std::map<std::string, std::string> const * params, std::map<std::string, std::string> const * headers, std::string * response, std::map<std::string, std::string> *resHeaders, const char *body, int len, const std::map<std::string, std::string> *formUrlencoded /* = nullptr */, const std::vector<formdata::TFormData> *formData /* = nullptr */, int *resCode /* = nullptr */, bool isRedir /* = false */, int reSendTimeoutMS) const
{
	if (m_mgr == nullptr) return COMERR_NOT_INIT;
	CCurlConnect *conn = m_mgr->connect(this->connect_timeout, this->socket_timeout);
	if (conn == nullptr) return COMERR_NULL_POINTER;
	return conn->send(url, params, body, len, headers, response, "POST", isRedir, resCode, resHeaders, formUrlencoded, formData, reSendTimeoutMS);
}

int CCurlHttpClient::put(std::string url, std::map<std::string, std::string> const * params, std::map<std::string, std::string> const * headers, std::string * response, std::map<std::string, std::string> *resHeaders, const char *body, int len, const std::map<std::string, std::string> *formUrlencoded /* = nullptr */, const std::vector<formdata::TFormData> *formData /* = nullptr */, int *resCode /* = nullptr */, bool isRedir /* = false */, int reSendTimeoutMS) const
{
	if (m_mgr == nullptr) return COMERR_NOT_INIT;
	CCurlConnect *conn = m_mgr->connect(this->connect_timeout, this->socket_timeout);
	if (conn == nullptr) return COMERR_NULL_POINTER;
	return conn->send(url, params, body, len, headers, response, "PUT", isRedir, resCode, resHeaders, formUrlencoded, formData, reSendTimeoutMS);
}

void CCurlHttpClient::addMap(const std::map<std::string, std::string> &src, std::map<std::string, std::string> &dst)
{
	for (auto iter = src.begin(); iter != src.end(); ++iter) {
		dst.insert(std::make_pair(iter->first, iter->second));
	}
}

int CCurlHttpClient::uploadFile(const std::string &url, const std::string &path
	, std::string &response
	, const std::map<std::string, std::string> *paramMap
	, const std::map<std::string, std::string> *headerMap
	, int bufSize /* = upload_buf_size */, const char *upload_isend /* = "isend" */, const char *upload_context /* = "context" */, const char *upload_fileext /* = "ext" */)
{
	int ret = COMERR_OK;
	std::string context = "";
	randtool::CUuid tool;
	ospathtool::COsPath ospath;
	context = tool.generator();
	if (context == "") return -1;
	std::ifstream readFile(path, std::ios::binary);
	if (!readFile) return -1;
	std::string ext;
	std::string dev;
	std::string mid;
	std::string filename;
	ospath.splitepath(path, dev, mid, filename, ext);
	while (1)
	{
		char *buf = new char[bufSize];
		memset(buf, 0, bufSize);
		readFile.read(buf, bufSize);
		int n = static_cast<int>(readFile.gcount());
		if (n <= 0)
		{
			std::map<std::string, std::string> headers;
			headers.insert(std::make_pair(upload_context, context));
			headers.insert(std::make_pair(upload_fileext, ext));
			headers.insert(std::make_pair(upload_isend, "1"));
			if (headerMap != nullptr) {
				addMap(*headerMap, headers);
			}
			std::map<std::string, std::string> params;
			if (paramMap != nullptr) {
				addMap(*paramMap, params);
			}
			std::string channels;
			int code = post(url, &params, buf, n, &headers, &response);
			if (code != CURLE_OK)
			{
				if (buf != nullptr) { delete[] buf; buf = nullptr; }
				readFile.close();
				return -1;
			}
			break;
		}
		std::map<std::string, std::string> headers;
		headers.insert(std::make_pair(upload_isend, "0"));
		headers.insert(std::make_pair(upload_context, context));
		headers.insert(std::make_pair(upload_fileext, ext));
		if (headerMap != nullptr) {
			addMap(*headerMap, headers);
		}
		std::map<std::string, std::string> params;
		if (paramMap != nullptr) {
			addMap(*paramMap, params);
		}
		std::string res = "";
		int ret = post(url, &params, buf, n, &headers, &res);
		if (ret != 0)
		{
			if (buf != nullptr) { delete[] buf; buf = nullptr; }
			readFile.close();
			return -1;
		}
		if (buf != nullptr) { delete[] buf; buf = nullptr; }
	}
	readFile.close();

	return ret;
}

int CCurlHttpClient::downloadBase64(const std::string &url, const std::string &context, std::string &base64
	, int bufSize, const char *download_context, const char *download_buflen, const char *download_rellen, const char *download_isend)
{
	int ret = COMERR_OK;
	base64 = "";
	mediatool::CImage image;
	while (true) {
		std::string res;
		int rellen = 0;
		bool b = singleDownload(url, context, res, rellen, bufSize, download_context, download_buflen, download_rellen, download_isend);
		if (b == false) break;
		base64.append(image.base64Encode(res.c_str(), rellen));
	}

	return ret;
}

int CCurlHttpClient::downloadFile(const std::string &url, const std::string &context, const std::string &filepath
	, int bufSize, const char *download_context, const char *download_buflen, const char *download_rellen, const char *download_isend)
{
	int ret = COMERR_OK;
	ospathtool::COs os;
	ospathtool::COsPath ospath;
	std::string dir;
	bool b = ospath.dirname(filepath, dir);
	if (b == true)
	{
		bool exist = ospath.exists(dir);
		if (exist == false)
		{
			os.mkdirs(dir);
		}
	}
	std::ofstream file(filepath, std::ios::binary|std::ios::app);
	if (!file) {
		return COMERR_FILE;
	}
	while (true) {
		std::string res;
		int rellen = 0;
		bool b = singleDownload(url, context, res, rellen, bufSize, download_context, download_buflen, download_rellen, download_isend);
		if (b == false) break;
		file.write(res.c_str(), res.size());
	}
	file.close();

	return ret;
}

bool CCurlHttpClient::singleDownload(const std::string &url, const std::string &context, std::string &res, int &rellen
	, int bufSize, const char *download_context, const char *download_buflen, const char *download_rellen, const char *download_isend)
{
	int ret = COMERR_OK;
	std::map<std::string, std::string> headers;
	headers.insert(std::make_pair(download_context, context));
	headers.insert(std::make_pair(download_buflen, std::to_string(bufSize)));
	std::map<std::string, std::string> resHeader;
	// std::unique_ptr<char[]> ptr = get(url, nullptr, &headers, bufSize, resHeader, ret);
	ret = get(url, nullptr, &headers, &res, resHeader);
	auto iter = resHeader.find(download_isend);
	if (iter == resHeader.end()) return false;
	auto iter2 = resHeader.find(download_rellen);
	if (iter2 == resHeader.end()) return false;
	if (ret != CURLE_OK || iter->second == "1") {
		return false;
	}
	rellen = stoi(iter2->second);
	return true;
}

}

