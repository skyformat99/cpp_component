#ifndef __CURL_CONN_MGR_H__
#define __CURL_CONN_MGR_H__

#include <mutex>
#include <list>
#include <condition_variable>

typedef void CURLSH;

namespace http
{

class CCurlConnect;
class CCurlConnMgr
{
public:
	explicit CCurlConnMgr(
		const char *cainfoPath = nullptr
		, const char *sslcertPath = nullptr
		, const char *keypassword = nullptr
		, int maxConn = 50);
	virtual ~CCurlConnMgr();

public:
	CCurlConnect *connect(int connTimeout, int sockTimeout);
	void disconnect(CCurlConnect *conn);
	CURLSH* sharedHandle() { return m_sharedHandle; }
private:
	CCurlConnect *create();
private:
	int m_max;
	std::list<CCurlConnect*> m_queue;
	std::mutex m_mutex;
	int m_count;
	CURLSH* m_sharedHandle;
	std::string m_cainfoPath;
	std::string m_sslcertPath;
	std::string m_keypassword;
};

}

#endif
