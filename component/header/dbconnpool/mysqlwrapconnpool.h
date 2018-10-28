#ifndef __MYSQL_WRAP_CONN_POOL_H__
#define __MYSQL_WRAP_CONN_POOL_H__

#include <stdint.h>
#include <list>
#include <mutex>
#include <string>
#include <condition_variable>
#include "Database.h"
#include "UTFail.h"
#include "AdhocStatement.h"

namespace dbconnpool
{

class CMysqlwrapConnPool
{
public:
	explicit CMysqlwrapConnPool(int max = 50);
	virtual ~CMysqlwrapConnPool();

public:
	MySQLWrap::Database *connect(const std::string &host
		, uint32_t port, const std::string &username
		, const std::string &userpwd, const std::string &dbname);
	void freeConnect(MySQLWrap::Database *db);

private:
	uint32_t m_max;
	uint32_t m_busyCount;
	std::list<MySQLWrap::Database*> m_queue;
	std::mutex m_queueMutex;
	std::condition_variable m_queueCV;
};

}

#endif
