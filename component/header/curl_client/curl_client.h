#ifndef __CURL_CLIENT_COMM_H__
#define __CURL_CLIENT_COMM_H__

#include <string>
#include <map>
#include <memory>
#include <vector>
#include "curl_param.h"

namespace Json
{
class Value;
}

struct curl_slist;

namespace http
{

const static int upload_buf_size = 32 * 1024;
const static int download_buf_size = 32 * 1024;

class CCurlConnMgr;
class CCurlHttpClient
{
public:
	explicit CCurlHttpClient();
	virtual ~CCurlHttpClient();

public:
	static int init(int max = 500
		, const char *cainfoPath = nullptr
		, const char *sslcertPath = nullptr
		, const char *keypassword = nullptr);

public:
	void setConnectTimeout(int connect_timeout);
	void setSocketTimeout(int socket_timeout);
	int uploadFile(const std::string &url, const std::string &path
		, std::string &response
		, const std::map<std::string, std::string> *paramMap = nullptr
		, const std::map<std::string, std::string> *headerMap = nullptr
		, int bufSize = upload_buf_size, const char *upload_isend = "isend", const char *upload_context = "context", const char *upload_fileext = "ext");
	int downloadBase64(const std::string &url, const std::string &context, std::string &base64
		, int bufSize = download_buf_size, const char *download_context = "context", const char *download_buflen = "buflen", const char *download_rellen = "rellen", const char *download_isend = "isend");
	int downloadFile(const std::string &url, const std::string &context, const std::string &filepath
		, int bufSize = download_buf_size, const char *download_context = "context", const char *download_buflen = "buflen", const char *download_rellen = "rellen", const char *download_isend = "isend");
	int get(
		std::string url
		, std::map<std::string, std::string> const * params
		, std::map<std::string, std::string> const * headers
		, std::string * response, int *resCode = nullptr
		, bool isRedir = false
		, int reSendTimeoutMs = -1) const;
	int get(
		std::string url
		, std::map<std::string, std::string> const * params
		, std::map<std::string, std::string> const * headers
		, std::string * response, std::map<std::string, std::string> &resHeaders
		, bool isRedir = false
		, int reSendTimeoutMS = -1) const;
	int post(
		std::string url
		, std::map<std::string, std::string> const * params
		, const std::string & body
		, std::map<std::string, std::string> const * headers
		, std::string * response, int *resCode = nullptr
		, bool isRedir = false
		, int reSendTimeoutMS = -1) const;
	int post(
		std::string url
		, std::map<std::string, std::string> const * params
		, const std::string & body
		, std::map<std::string, std::string> const * headers
		, std::string * response
		, std::map<std::string, std::string> &resHeaders
		, int *resCode = nullptr
		, bool isRedir = false
		, int reSendTimeoutMS = -1) const;
	int put(
		std::string url
		, std::map<std::string, std::string> const * params
		, const std::string & body
		, std::map<std::string, std::string> const * headers
		, std::string * response, int *resCode = nullptr
		, bool isRedir = false
		, int reSendTimeoutMS = -1) const;
	int patch(
		std::string url
		, std::map<std::string, std::string> const * params
		, const std::string & body
		, std::map<std::string, std::string> const * headers
		, std::string * response, int *resCode = nullptr
		, bool isRedir = false
		, int reSendTimeoutMS = -1) const;
	int del(
		std::string url
		, std::map<std::string, std::string> const * params
		, std::map<std::string, std::string> const * headers
		, std::string * response, int *resCode = nullptr
		, bool isRedir = false
		, int reSendTimeoutMS = -1) const;
	int head(
		std::string url
		, std::map<std::string, std::string> const * params
		, std::map<std::string, std::string> const * headers
		, std::string * response, int *resCode = nullptr
		, bool isRedir = false
		, int reSendTimeoutMS = -1) const;
	int updated(
		std::string url
		, std::map<std::string, std::string> const * params
		, std::map<std::string, std::string> const * headers
		, std::string * response, int *resCode = nullptr
		, bool isRedir = false
		, int reSendTimeoutMS = -1) const;
	int deleted(
		std::string url
		, std::map<std::string, std::string> const * params
		, std::map<std::string, std::string> const * headers
		, std::string * response, int *resCode = nullptr
		, bool isRedir = false
		, int reSendTimeoutMS = -1) const;
	int post(
		std::string url
		, std::map<std::string, std::string> const * params
		, const char *body, int len
		, std::map<std::string, std::string> const * headers
		, std::string * response, int *resCode = nullptr
		, bool isRedir = false
		, int reSendTimeoutMS = -1) const;

	int post(
		std::string url
		, std::map<std::string, std::string> const * params
		, std::map<std::string, std::string> const * headers
		, std::string * response
		, std::map<std::string, std::string> *resHeaders
		, const char *body, int len
		, const std::map<std::string, std::string> *formUrlencoded = nullptr
		, const std::vector<formdata::TFormData> *formData = nullptr
		, int *resCode = nullptr
		, bool isRedir = false
		, int reSendTimeoutMS = -1) const;
	int put(
		std::string url
		, std::map<std::string, std::string> const * params
		, std::map<std::string, std::string> const * headers
		, std::string * response
		, std::map<std::string, std::string> *resHeaders
		, const char *body, int len
		, const std::map<std::string, std::string> *formUrlencoded = nullptr
		, const std::vector<formdata::TFormData> *formData = nullptr
		, int *resCode = nullptr
		, bool isRedir = false
		, int reSendTimeoutMS = -1) const;

private:
	bool singleDownload(const std::string &url, const std::string &context, std::string &res, int &rellen
		, int bufSize = download_buf_size, const char *download_context = "context", const char *download_buflen = "buflen", const char *download_rellen = "rellen", const char *download_isend = "isend");
	void addMap(const std::map<std::string, std::string> &src
		, std::map<std::string, std::string> &dst);

private:
	int connect_timeout;
	int socket_timeout;
	static CCurlConnMgr *m_mgr;
};

}

#endif // __CURL_CLIENT_COMM_H__

