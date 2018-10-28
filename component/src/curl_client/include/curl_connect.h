#ifndef __CURL_CONNECT_H__
#define __CURL_CONNECT_H__

#include <string>
#include <map>
#include <memory>
#include <stdint.h>
#include <vector>
#include "curl/curl.h"
#include "curl_param.h"

namespace http
{

class CCurlConnMgr;
class CCurlConnect
{
public:
	explicit CCurlConnect(CCurlConnMgr *mgr
		, const char *cainfoPath = nullptr
		, const char *sslcertPath = nullptr
		, const char *keypassword = nullptr);
	virtual ~CCurlConnect();

public:
	void setConnectTimeout(int connect_timeout);
	void setSocketTimeout(int socket_timeout);
	int send(
		const std::string &url
		, std::map<std::string, std::string> const * params
		, const char *body, size_t len
		, std::map<std::string, std::string> const * headers
		, std::string * response, const char *custom
		, bool isRedir = false
		, int *resCode = nullptr
		, std::map<std::string, std::string> *resheaders = nullptr
		, const std::map<std::string, std::string> *formUrlencoded = nullptr
		, const std::vector<formdata::TFormData> *formData = nullptr
		, int reSendTimeoutMS = -1);

	friend class CCurlConnMgr;
private:
	void setIsUse(bool isuse);
	bool isUse() const;
	void makeUrlencodedForm(std::map<std::string, std::string> const & params, std::string * content) const;
	void appendUrlParams(std::map<std::string, std::string> const & params, std::string* url) const;
	void appendHeaders(std::map<std::string, std::string> const & headers, curl_slist ** slist) const;
	void parseHeaders(const std::string &str, std::map<std::string, std::string> &headers);
	void joinFormdata(const std::map<std::string, formdata::TFormData> &datas, std::string &joinResult);
	void joinFormUrlencoded(const std::map<std::string, std::string> &datas, std::string &joinResult);

private:
	CURL *m_url;
	CCurlConnMgr *m_mgr;
	int m_connTimeout;
	int m_sockTimeout;
	bool m_isuse;
};

}

#endif
