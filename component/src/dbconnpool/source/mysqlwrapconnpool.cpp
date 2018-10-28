#include "dbconnpool/mysqlwrapconnpool.h"

namespace dbconnpool
{

CMysqlwrapConnPool::CMysqlwrapConnPool(int max)
	: m_max(max)
	, m_busyCount(0)
	, m_queue()
	, m_queueMutex()
	, m_queueCV()
{
}

CMysqlwrapConnPool::~CMysqlwrapConnPool()
{
	for (auto iter = m_queue.begin(); iter != m_queue.end(); ++iter)
	{
		if ((*iter) != nullptr) {
			delete *iter;
			*iter = nullptr;
		}
	}
}

MySQLWrap::Database *CMysqlwrapConnPool::connect(
	const std::string &host, uint32_t port, const std::string &username
	, const std::string &userpwd, const std::string &dbname)
{
	MySQLWrap::Database *db = nullptr;
	do 
	{
		if (m_busyCount >= m_max) {
			std::unique_lock<std::mutex> lk(m_queueMutex);
			m_queueCV.wait(lk);
		}
		m_queueMutex.lock();
		if (m_queue.empty() == false) {
			db = m_queue.front();
			m_queue.pop_front();
			++m_busyCount;
		}
		if (m_queue.empty() == true && m_busyCount < m_max) {
			db = new MySQLWrap::Database(host, username, userpwd, dbname, port, NULL, 0);
			db->Connect();
			++m_busyCount;
		}
		m_queueMutex.unlock();
	} while (db == nullptr);
	return db;
}

void CMysqlwrapConnPool::freeConnect(MySQLWrap::Database *db)
{
	m_queueMutex.lock();
	m_queue.push_back(db);
	--m_busyCount;
	m_queueMutex.unlock();
	m_queueCV.notify_all();
}

}
