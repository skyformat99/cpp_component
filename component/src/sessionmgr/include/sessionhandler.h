#ifndef __SESSION_HANDLER_H__
#define __SESSION_HANDLER_H__

#include <map>
#include <string>
#include <mutex>
#include <thread>
#include <stdint.h>

namespace sessionmgr_db
{
class CDbHandler;
}

namespace session
{

struct TSessionInfo
{
	std::string sessionId;
	uint64_t timeoutTime;
	uint64_t loseVaildTime;
};

class CSessionHandler
{
public:
	explicit CSessionHandler();
	virtual ~CSessionHandler();

public:
	virtual bool createSession(uint64_t timeout, std::string &sessionId);
	virtual bool destroySession(const std::string &sessionId);
	virtual bool sessionIsVaild(const std::string &sessionId, bool &isVaild);
	virtual bool resetLoseVaildTime(const std::string &sessionId);

protected:
	void init();
	uint64_t getNowTimeStamp();
	virtual void sessions(std::map<std::string, TSessionInfo> &sessionInfos) = 0;

private:
	void loadSessions();
	void handleSession();
	void insertSessionToMemory(const std::string &sessionId, uint64_t timeout);
	bool updateLoseVaildTimeToMemory(const std::string &sessionId, uint64_t loseVaildTime);
	bool deleteSessionFromMemory(const std::string &sessionId);
	bool getSessionInfoFromMemory(const std::string &sessionId, TSessionInfo &info);

private:
	bool m_isRun;
	std::thread m_timerThread;
	std::map<std::string, TSessionInfo> m_sessions;
	std::mutex m_mutex;
};

class CSqliteSessionHandler : public CSessionHandler
{
public:
	explicit CSqliteSessionHandler(bool isMemory = false);
	virtual ~CSqliteSessionHandler();

public:
	virtual bool createSession(uint64_t timeout, std::string &sessionId);
	virtual bool destroySession(const std::string &sessionId);
	virtual bool sessionIsVaild(const std::string &sessionId, bool &isVaild);
	virtual bool resetLoseVaildTime(const std::string &sessionId);

protected:
	virtual void sessions(std::map<std::string, TSessionInfo> &sessionInfos);

private:
	sessionmgr_db::CDbHandler *m_handler;
};

class CMemorySessionHandler : public CSqliteSessionHandler
{
public:
	explicit CMemorySessionHandler();
	virtual ~CMemorySessionHandler();
};

class CMysqlSessionHandler : public CSessionHandler
{
public:
	explicit CMysqlSessionHandler();
	virtual ~CMysqlSessionHandler();

public:
	virtual bool createSession(uint64_t timeout, std::string &sessionId);
	virtual bool destroySession(const std::string &sessionId);
	virtual bool sessionIsVaild(const std::string &sessionId, bool &isVaild);
	virtual bool resetLoseVaildTime(const std::string &sessionId);

protected:
	virtual void sessions(std::map<std::string, TSessionInfo> &sessionInfos);
};

}

#endif
