#include <stdio.h>
#include <string.h>
#include <sstream>
#include "sqlite3.h"
#include "sessionmgr_db_handler.h"

namespace sessionmgr_db
{

CDbHandler::CDbHandler(const std::string &dbpath, bool isMemory)
	: m_db(nullptr)
	, m_mutex()
{
	sqlite3_threadsafe();
	sqlite3_config(SQLITE_CONFIG_MULTITHREAD);
	int ret = SQLITE_OK;
	if (isMemory == false) {
		ret = sqlite3_open(dbpath.c_str(), &m_db);
	}
	else {
		ret = sqlite3_open(":memory:", &m_db);
	}
	if (ret == SQLITE_OK) {
		std::string sql = "\
create table if not exists t_session_info (\
	id integer primary key autoincrement,\
 	sessionid varchar(64),\
 	timeouttime bigint,\
 	losevaildtime bigint\
);";
		sqlite3_exec(m_db, sql.c_str(), nullptr, nullptr, nullptr);
	}
}

CDbHandler::~CDbHandler()
{
	sqlite3_close(m_db);
}

uint32_t CDbHandler::createSession(const CCreateSessionInput &input)
{
	if (m_db == nullptr) return -1;
	uint32_t ret = 0;

	char buf[512];
	memset(buf, 0, sizeof(buf));
	snprintf(buf, sizeof(buf), "insert into t_session_info values(null, \"%s\", \"%llu\", \"%llu\");"
		, input.getSessionId().c_str(), input.getTimeoutTime(), input.getLosevaildtime());
	std::string sql = buf;
	m_mutex.lock();
	ret = sqlite3_exec(m_db, sql.c_str(), nullptr, nullptr, nullptr);
	m_mutex.unlock();

	return ret;
}

uint32_t CDbHandler::destroySession(const CDestroySessionInput &input)
{
	if (m_db == nullptr) return -1;
	uint32_t ret = 0;

	char buf[512];
	memset(buf, 0, sizeof(buf));
	snprintf(buf, sizeof(buf), "delete from t_session_info\
		where sessionid = \"%s\";"
		, input.getSessionId().c_str());
	std::string sql = buf;
	m_mutex.lock();
	ret = sqlite3_exec(m_db, sql.c_str(), nullptr, nullptr, nullptr);
	m_mutex.unlock();

	return ret;
}

int getSessionIdCountCallback(void *data, int colCount, char **colValue, char **colName)
{
	int ret = 0;

	CGetSessionIdCountOutput *p = reinterpret_cast<CGetSessionIdCountOutput*>(data);
	if (p == nullptr) return -1;
	std::stringstream ss;
	if (colValue[0] != nullptr) {
		int count = 0;
		ss << colValue[0];
		ss >> count;
		ss.clear();
		p->setCount(count);
	}

	return ret;
}
uint32_t CDbHandler::getSessionIdCount(const CGetSessionIdCountInput &input, CGetSessionIdCountOutput &output)
{
	if (m_db == nullptr) return -1;
	uint32_t ret = 0;

	char buf[512];
	memset(buf, 0, sizeof(buf));
	snprintf(buf, sizeof(buf), "select count(*) from t_session_info\
		where sessionid = \"%s\"\
		limit 1;"
		, input.getSessionId().c_str());
	std::string sql = buf;
	m_mutex.lock();
	ret = sqlite3_exec(m_db, sql.c_str(), getSessionIdCountCallback, &output, nullptr);
	m_mutex.unlock();

	return ret;
}

uint32_t CDbHandler::updateLoseVaildTime(const CUpdateLoseVaildTimeInput &input)
{
	if (m_db == nullptr) return -1;
	uint32_t ret = 0;

	char buf[512];
	memset(buf, 0, sizeof(buf));
	snprintf(buf, sizeof(buf), "update t_session_info set losevaildtime = \"%llu\"\
		where sessionid = \"%s\";"
		, input.getLosevaildtime(), input.getSessionId().c_str());
	std::string sql = buf;
	m_mutex.lock();
	ret = sqlite3_exec(m_db, sql.c_str(), nullptr, nullptr, nullptr);
	m_mutex.unlock();

	return ret;
}

int getSessionInfoCallback(void *data, int colCount, char **colValue, char **colName)
{
	int ret = 0;

	CGetSessionInfoOutput *p = reinterpret_cast<CGetSessionInfoOutput*>(data);
	if (p == nullptr) return -1;
	std::stringstream ss;
	if (colValue[0] != nullptr) {
		int id = 0;
		ss << colValue[0];
		ss >> id;
		ss.clear();
		p->setId(id);
	}
	if (colValue[1] != nullptr) {
		p->setSessionId(colValue[1]);
	}
	if (colValue[2] != nullptr) {
		unsigned long long timeoutTime = 0;
		ss << colValue[2];
		ss >> timeoutTime;
		ss.clear();
		p->setTimeoutTime(timeoutTime);
	}
	if (colValue[3] != nullptr) {
		unsigned long long losevaildtime = 0;
		ss << colValue[3];
		ss >> losevaildtime;
		ss.clear();
		p->setLosevaildtime(losevaildtime);
	}

	return ret;
}
uint32_t CDbHandler::getSessionInfo(const CGetSessionInfoInput &input, CGetSessionInfoOutput &output)
{
	if (m_db == nullptr) return -1;
	uint32_t ret = 0;

	char buf[512];
	memset(buf, 0, sizeof(buf));
	snprintf(buf, sizeof(buf), "select * from t_session_info\
		where sessionid = \"%s\";"
		, input.getSessionId().c_str());
	std::string sql = buf;
	m_mutex.lock();
	ret = sqlite3_exec(m_db, sql.c_str(), getSessionInfoCallback, &output, nullptr);
	m_mutex.unlock();

	return ret;
}

int getAllSessionInfoCallback(void *data, int colCount, char **colValue, char **colName)
{
	int ret = 0;

	std::list<CGetAllSessionInfoOutput> *p = reinterpret_cast<std::list<CGetAllSessionInfoOutput>*>(data);
	if (p == nullptr) return -1;
	CGetAllSessionInfoOutput tmp;
	std::stringstream ss;
	if (colValue[0] != nullptr) {
		int id = 0;
		ss << colValue[0];
		ss >> id;
		ss.clear();
		tmp.setId(id);
	}
	if (colValue[1] != nullptr) {
		tmp.setSessionId(colValue[1]);
	}
	if (colValue[2] != nullptr) {
		unsigned long long timeoutTime = 0;
		ss << colValue[2];
		ss >> timeoutTime;
		ss.clear();
		tmp.setTimeoutTime(timeoutTime);
	}
	if (colValue[3] != nullptr) {
		unsigned long long losevaildtime = 0;
		ss << colValue[3];
		ss >> losevaildtime;
		ss.clear();
		tmp.setLosevaildtime(losevaildtime);
	}
	p->push_back(tmp);

	return ret;
}
uint32_t CDbHandler::getAllSessionInfo(std::list<CGetAllSessionInfoOutput> &output)
{
	if (m_db == nullptr) return -1;
	uint32_t ret = 0;

	std::string sql = "select * from t_session_info;";
	m_mutex.lock();
	ret = sqlite3_exec(m_db, sql.c_str(), getAllSessionInfoCallback, &output, nullptr);
	m_mutex.unlock();

	return ret;
}

/*@@start@@*/

}

