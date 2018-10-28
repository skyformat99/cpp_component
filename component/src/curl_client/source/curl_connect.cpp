#include "timetool/timer.h"
#include "stringtool/stringtool.h"
#include "ospathtool/ospathtool.h"
#include "curl_connect.h"
#include "curl_conn_mgr.h"
#include <string.h>
#include <sstream>
#include <thread>

namespace http
{

inline size_t onWriteString(void * buffer, size_t size, size_t nmemb, void * userp)
{
	std::string * str = dynamic_cast<std::string *>((std::string *)userp);
	str->append((char *)buffer, size * nmemb);
	return nmemb;
}

CCurlConnect::CCurlConnect(CCurlConnMgr *mgr , const char *cainfoPath /* = nullptr  */, const char *sslcertPath /* = nullptr  */, const char *keypassword /* = nullptr */)
	: m_url(nullptr)
	, m_mgr(mgr)
	, m_connTimeout(0)
	, m_sockTimeout(0)
	, m_isuse(false)
{
	m_url = curl_easy_init();
	curl_easy_setopt(m_url, CURLOPT_SHARE, mgr->sharedHandle());
	curl_easy_setopt(m_url, CURLOPT_DNS_CACHE_TIMEOUT, 60 * 5);
	if (cainfoPath == nullptr && sslcertPath == nullptr && keypassword == nullptr) {
	}
	else {
		curl_easy_setopt(m_url, CURLOPT_SSL_VERIFYPEER, false);
		curl_easy_setopt(m_url, CURLOPT_SSL_VERIFYHOST, false);
	}
	if (cainfoPath != nullptr)
	{
		curl_easy_setopt(m_url, CURLOPT_CAINFO, cainfoPath);
	}
	if (sslcertPath != nullptr) {
		curl_easy_setopt(m_url, CURLOPT_SSLCERT, sslcertPath);
	}
	if (keypassword != nullptr) {
		curl_easy_setopt(m_url, CURLOPT_KEYPASSWD, keypassword);
	}
}

CCurlConnect::~CCurlConnect()
{
	curl_easy_cleanup(m_url);
}

void CCurlConnect::setIsUse(bool isuse)
{
	m_isuse = isuse;
}

bool CCurlConnect::isUse() const
{
	return m_isuse;
}

void CCurlConnect::setConnectTimeout(int connect_timeout)
{
	m_connTimeout = connect_timeout;
}

void CCurlConnect::setSocketTimeout(int socket_timeout)
{
	m_sockTimeout = socket_timeout;
}

void CCurlConnect::makeUrlencodedForm(std::map<std::string, std::string> const & params, std::string * content) const
{
	content->clear();
	std::map<std::string, std::string>::const_iterator it;
	for(it=params.begin(); it!=params.end(); it++)
	{
		if (it != params.begin())
		{
			*content += '&';
		}

		char * key = curl_escape(it->first.c_str(), (int) it->first.size());
		char * value = curl_escape(it->second.c_str(),(int) it->second.size());
		*content += key;
		*content += '=';
		*content += value;
		curl_free(key);
		curl_free(value);
	}
}

void CCurlConnect::appendUrlParams(std::map<std::string, std::string> const & params, std::string* url) const
{
	if(params.empty()) {
		return;
	}
	std::string content;
	this->makeUrlencodedForm(params, &content);
	bool url_has_param = false;
	for (int i = 0; i < url->size(); ++i) {
		char ch = url->at(i);
		if (ch == '?') {
			url_has_param = true;
			break;
		}
	}
	if (url_has_param) {
		url->append("&");
	} else {
		url->append("?");
	}
	url->append(content);
}

void CCurlConnect::appendHeaders(std::map<std::string, std::string> const & headers, curl_slist ** slist) const
{
	std::ostringstream ostr;
	std::map<std::string, std::string>::const_iterator it;
	for(it=headers.begin(); it!=headers.end(); it++)
	{
		ostr << it->first << ":" << it->second;
		*slist = curl_slist_append(*slist, ostr.str().c_str());
		ostr.str("");
	}
}

int CCurlConnect::send(
	const std::string &url
	, std::map<std::string, std::string> const * params
	, const char *body, size_t len
	, std::map<std::string, std::string> const * headers
	, std::string * response, const char *custom
	, bool isRedir
	, int *resCode
	, std::map<std::string, std::string> * resheaders
	, const std::map<std::string, std::string> *formUrlencoded
	, const std::vector<formdata::TFormData> *formData
	, int reSendTimeoutMS)
{
	std::string u(url);
	struct curl_slist * slist = nullptr;
	struct curl_httppost *formpost = nullptr;
	struct curl_httppost *lastptr = nullptr;

	if (formUrlencoded != nullptr) {
		std::stringstream ss;
		ss << "Content-Type" << ":" << "application/x-www-form-urlencoded";
		slist = curl_slist_append(slist, ss.str().c_str());
	}

	if (headers != nullptr) {
		this->appendHeaders(*headers, &slist);
	}
#if 0
	else if (formData != nullptr) {
		std::stringstream ss;
		ss << "Content-Type" << ":" << "multipart/form-data; boundary=----WebKitFormBoundary7MA4YWxkTrZu0gW";
		slist = curl_slist_append(slist, ss.str().c_str());
		if (headers == nullptr) {
			std::map<std::string, std::string> hs;
			this->appendHeaders(hs, &slist);
		}
		else {
			this->appendHeaders(*headers, &slist);
		}
	}
#endif
	if (params) {
		this->appendUrlParams(*params, &u);
	}

	std::string formUrlencodedResult;
	std::string formDataResult;
	curl_easy_setopt(m_url, CURLOPT_URL, u.c_str());
	curl_easy_setopt(m_url, CURLOPT_HTTPHEADER, slist);
	curl_easy_setopt(m_url, CURLOPT_CUSTOMREQUEST, custom);
	if (formUrlencoded != nullptr) {
		joinFormUrlencoded(*formUrlencoded, formUrlencodedResult);
		curl_easy_setopt(m_url, CURLOPT_POSTFIELDS, formUrlencodedResult.c_str());
		curl_easy_setopt(m_url, CURLOPT_POSTFIELDSIZE, formUrlencodedResult.size());
	}
	else if (formData != nullptr) {
#if 0
		joinFormdata(*formData, formDataResult);
		curl_easy_setopt(m_url, CURLOPT_POSTFIELDS, formDataResult.c_str());
		curl_easy_setopt(m_url, CURLOPT_POSTFIELDSIZE, formDataResult.size());
#endif
		for (auto iter = (*formData).begin(); iter != (*formData).end(); ++iter) {
			if (iter->type == formdata::EFORMDATA_TYPE_FILE) {
				curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, iter->formdataName.c_str(), CURLFORM_FILE, iter->filenameORvalue.c_str(), CURLFORM_CONTENTTYPE, "application/octet-stream", CURLFORM_END);
			}
			else if (iter->type == formdata::EFORMDATA_TYPE_TEXT) {
				curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, iter->formdataName.c_str(), CURLFORM_COPYCONTENTS, iter->filenameORvalue.c_str(), CURLFORM_END);
			}
		}
		curl_easy_setopt(m_url, CURLOPT_HTTPPOST, formpost);
	}
	else if (body != nullptr || len > 0)
	{
		curl_easy_setopt(m_url, CURLOPT_POSTFIELDS, body);
		curl_easy_setopt(m_url, CURLOPT_POSTFIELDSIZE, len);
	}
	else
	{
		curl_easy_setopt(m_url, CURLOPT_POSTFIELDS, 0);
		curl_easy_setopt(m_url, CURLOPT_POSTFIELDSIZE, 0);
	}
	curl_easy_setopt(m_url, CURLOPT_WRITEFUNCTION, onWriteString);
	curl_easy_setopt(m_url, CURLOPT_WRITEDATA, (void *) response);
	std::string resHeaderStr;
	curl_easy_setopt(m_url, CURLOPT_HEADERFUNCTION, onWriteString);
	curl_easy_setopt(m_url, CURLOPT_WRITEHEADER, (void*)&resHeaderStr);
	curl_easy_setopt(m_url, CURLOPT_CONNECTTIMEOUT_MS, m_connTimeout);
	curl_easy_setopt(m_url, CURLOPT_TIMEOUT_MS, m_sockTimeout);
	curl_easy_setopt(m_url, CURLOPT_NOSIGNAL, 1L);
	curl_easy_setopt(m_url, CURLOPT_SSL_VERIFYPEER, false);
	curl_easy_setopt(m_url, CURLOPT_SSL_VERIFYHOST, true);
	curl_easy_setopt(m_url, CURLOPT_VERBOSE, false);
	
	int status_code = CURLE_OK;
	if (reSendTimeoutMS <= 0) {
		status_code = curl_easy_perform(m_url);
	}
	else {
		timetool::CTimer timer;
		timer.startTimer(0, [this, &status_code]() -> bool {
			status_code = curl_easy_perform(m_url);
			if (status_code == CURLE_OK) {
				return false;
			}
			return true;
		}, false, timetool::CTimer::timer_mode_wait, timetool::CTimer::thread_mode_join, 1000, reSendTimeoutMS);
	}

	if (resheaders != nullptr) {
		parseHeaders(resHeaderStr, *resheaders);
	}

	if (resCode != nullptr) {
		long code = 0;
		curl_easy_getinfo(m_url, CURLINFO_RESPONSE_CODE, &code);
		*resCode = (int)code;
	}

	if (isRedir == true) {
		// curl_easy_setopt(m_url, CURLOPT_FOLLOWLOCATION, 1);
		long code = 0;
		if (status_code == 0) {
			curl_easy_getinfo(m_url, CURLINFO_RESPONSE_CODE, &code);
			if (code == 302 || code == 301) {
				std::map<std::string, std::string> hm;
				parseHeaders(resHeaderStr, hm);
				auto iter = hm.begin();
				for (; iter != hm.end(); ++iter) {
					if (iter->first == "Location") break;
				}
				if (iter != hm.end()) {
					m_mgr->connect(m_connTimeout, m_sockTimeout)->send(iter->second, params, body, len, headers, response, custom, false);
				}
			}
		}
	}
	
	curl_formfree(formpost);
	curl_slist_free_all(slist);

	m_mgr->disconnect(this);
	
	return status_code;
}

void CCurlConnect::parseHeaders(const std::string &str, std::map<std::string, std::string> &headers)
{
	stringtool::CStringTools st;
	std::vector<std::string> lines = st.split(str, "\n");
	headers.clear();
	for (auto iter = lines.begin(); iter != lines.end(); ++iter) {
		std::string tmp = *iter;
		try
		{
			tmp.replace(iter->find("\r"), 1, "");
			std::vector<std::string> kv = st.split(tmp, ":");
			if (kv.size() < 2) continue;
			else if (kv.size() == 2) {
				headers.insert(std::make_pair(st.trim2(kv[0]), st.trim2(kv[1])));
			}
			else {
				std::string tmp;
				for (size_t i = 1; i < kv.size(); ++i) {
					tmp.append(kv[i]);
					if (i < kv.size() - 1) {
						tmp.append(":");
					}
				}
				headers.insert(std::make_pair(st.trim2(kv[0]), st.trim2(tmp)));
			}
		}
		catch (...)
		{
		}
	}
}

void CCurlConnect::joinFormdata(const std::map<std::string, formdata::TFormData> &datas, std::string &joinResult)
{
	std::stringstream ss;
	for (auto iter = datas.begin(); iter != datas.end(); ++iter) {
		ss << "------WebKitFormBoundary7MA4YWxkTrZu0gW\r\nContent-Disposition: form-data; name=\""
			<< iter->first << "\"";
		if (iter->second.type == formdata::EFORMDATA_TYPE_FILE) {
			ss << "; filename=\"" << iter->second.filenameORvalue << "\"";
			ss << "\r\nContent-Type:\r\n";
		}
		ss << "\r\n\r\n";
		if (iter->second.type == formdata::EFORMDATA_TYPE_TEXT) {
			ss << iter->second.filenameORvalue;
			ss << "\r\n";
		}
	}
	ss << "------WebKitFormBoundary7MA4YWxkTrZu0gW--";
	joinResult = ss.str();
}

void CCurlConnect::joinFormUrlencoded(const std::map<std::string, std::string> &datas, std::string &joinResult)
{
	std::stringstream ss;
	size_t size = datas.size();
	size_t i = 0;
	for (auto iter = datas.begin(); iter != datas.end(); ++iter) {
		++i;
		ss << iter->first << "=" << iter->second;
		if (i < size) {
			ss << "&";
		}
	}
	joinResult.assign(ss.str());
}

}
