#include <thread>
#include "curl_connect.h"
#include "curl_conn_mgr.h"

namespace http
{

CCurlConnMgr::CCurlConnMgr(const char *cainfoPath /* = nullptr  */, const char *sslcertPath /* = nullptr  */, const char *keypassword /* = nullptr  */, int maxConn /* = 50 */)
	: m_max(maxConn)
	, m_queue()
	, m_mutex()
	, m_count(0)
	, m_sharedHandle(nullptr)
	, m_cainfoPath("")
	, m_sslcertPath("")
	, m_keypassword("")
{
	if (cainfoPath != nullptr) m_cainfoPath = cainfoPath;
	if (sslcertPath != nullptr) m_sslcertPath = sslcertPath;
	if (keypassword != nullptr) m_keypassword = keypassword;
	curl_global_init(CURL_GLOBAL_ALL);
	m_sharedHandle = curl_share_init();
	curl_share_setopt(sharedHandle(), CURLSHOPT_SHARE, CURL_LOCK_DATA_DNS);
}

CCurlConnMgr::~CCurlConnMgr()
{
	m_mutex.lock();
	for (CCurlConnect* conn : m_queue)
	{
		delete conn;
	}
	m_queue.clear();
	m_mutex.unlock();
}

CCurlConnect *CCurlConnMgr::connect(int connTimeout, int sockTimeout)
{
	CCurlConnect *conn = nullptr;
	conn = create();
	conn->setConnectTimeout(connTimeout);
	conn->setSocketTimeout(sockTimeout);
	return conn;
}

CCurlConnect *CCurlConnMgr::create()
{
	CCurlConnect *conn = nullptr;
	do 
	{
		m_mutex.lock();
		if (!m_queue.empty())
		{
			conn = m_queue.front();
			m_queue.pop_front();
		}
		m_mutex.unlock();
		if (!conn && m_count < m_max)
		{
			conn = new CCurlConnect(this
				, (m_cainfoPath == "" ? nullptr : m_cainfoPath.c_str())
				, (m_sslcertPath == "" ? nullptr : m_sslcertPath.c_str())
				, (m_keypassword == "" ? nullptr : m_keypassword.c_str()));
			++m_count;
		}
		if (!conn)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
		}
	} while (!conn);

	return conn;
}

void CCurlConnMgr::disconnect(CCurlConnect *conn)
{
	m_mutex.lock();
	m_queue.push_back(conn);
	m_mutex.unlock();
}

}
