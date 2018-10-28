#ifndef __SESSIONMGR_DB_DB_PARAM_H__
#define __SESSIONMGR_DB_DB_PARAM_H__

#include <string>
#include <list>

namespace sessionmgr_db
{

class CCreateSessionInput
{
public:
	explicit CCreateSessionInput()
		: sessionId(""), timeoutTime(0), losevaildtime(0) {}
	explicit CCreateSessionInput(const std::string &sessionId, const unsigned long long &timeoutTime, const unsigned long long &losevaildtime)
		: sessionId(sessionId), timeoutTime(timeoutTime), losevaildtime(losevaildtime) {}
	virtual ~CCreateSessionInput() {}

public:
	void setSessionId(const std::string &sessionId) { this->sessionId = sessionId; }
	const std::string &getSessionId() const { return this->sessionId; }
	void setTimeoutTime(const unsigned long long &timeoutTime) { this->timeoutTime = timeoutTime; }
	const unsigned long long &getTimeoutTime() const { return this->timeoutTime; }
	void setLosevaildtime(const unsigned long long &losevaildtime) { this->losevaildtime = losevaildtime; }
	const unsigned long long &getLosevaildtime() const { return this->losevaildtime; }

private:
	std::string sessionId;
	unsigned long long timeoutTime;
	unsigned long long losevaildtime;
};

class CDestroySessionInput
{
public:
	explicit CDestroySessionInput()
		: sessionId("") {}
	explicit CDestroySessionInput(const std::string &sessionId)
		: sessionId(sessionId) {}
	virtual ~CDestroySessionInput() {}

public:
	void setSessionId(const std::string &sessionId) { this->sessionId = sessionId; }
	const std::string &getSessionId() const { return this->sessionId; }

private:
	std::string sessionId;
};

class CGetSessionIdCountInput
{
public:
	explicit CGetSessionIdCountInput()
		: sessionId("") {}
	explicit CGetSessionIdCountInput(const std::string &sessionId)
		: sessionId(sessionId) {}
	virtual ~CGetSessionIdCountInput() {}

public:
	void setSessionId(const std::string &sessionId) { this->sessionId = sessionId; }
	const std::string &getSessionId() const { return this->sessionId; }

private:
	std::string sessionId;
};

class CGetSessionIdCountOutput
{
public:
	explicit CGetSessionIdCountOutput()
		: count(0) {}
	explicit CGetSessionIdCountOutput(const int &count)
		: count(count) {}
	virtual ~CGetSessionIdCountOutput() {}

public:
	void setCount(const int &count) { this->count = count; }
	const int &getCount() const { return this->count; }

private:
	int count;
};

class CUpdateLoseVaildTimeInput
{
public:
	explicit CUpdateLoseVaildTimeInput()
		: sessionId(""), losevaildtime(0) {}
	explicit CUpdateLoseVaildTimeInput(const std::string &sessionId, const unsigned long long &losevaildtime)
		: sessionId(sessionId), losevaildtime(losevaildtime) {}
	virtual ~CUpdateLoseVaildTimeInput() {}

public:
	void setSessionId(const std::string &sessionId) { this->sessionId = sessionId; }
	const std::string &getSessionId() const { return this->sessionId; }
	void setLosevaildtime(const unsigned long long &losevaildtime) { this->losevaildtime = losevaildtime; }
	const unsigned long long &getLosevaildtime() const { return this->losevaildtime; }

private:
	std::string sessionId;
	unsigned long long losevaildtime;
};

class CGetSessionInfoInput
{
public:
	explicit CGetSessionInfoInput()
		: sessionId("") {}
	explicit CGetSessionInfoInput(const std::string &sessionId)
		: sessionId(sessionId) {}
	virtual ~CGetSessionInfoInput() {}

public:
	void setSessionId(const std::string &sessionId) { this->sessionId = sessionId; }
	const std::string &getSessionId() const { return this->sessionId; }

private:
	std::string sessionId;
};

class CGetSessionInfoOutput
{
public:
	explicit CGetSessionInfoOutput()
		: id(0), sessionId(""), timeoutTime(0), losevaildtime(0) {}
	explicit CGetSessionInfoOutput(const int &id, const std::string &sessionId, const unsigned long long &timeoutTime, const unsigned long long &losevaildtime)
		: id(id), sessionId(sessionId), timeoutTime(timeoutTime), losevaildtime(losevaildtime) {}
	virtual ~CGetSessionInfoOutput() {}

public:
	void setId(const int &id) { this->id = id; }
	const int &getId() const { return this->id; }
	void setSessionId(const std::string &sessionId) { this->sessionId = sessionId; }
	const std::string &getSessionId() const { return this->sessionId; }
	void setTimeoutTime(const unsigned long long &timeoutTime) { this->timeoutTime = timeoutTime; }
	const unsigned long long &getTimeoutTime() const { return this->timeoutTime; }
	void setLosevaildtime(const unsigned long long &losevaildtime) { this->losevaildtime = losevaildtime; }
	const unsigned long long &getLosevaildtime() const { return this->losevaildtime; }

private:
	int id;
	std::string sessionId;
	unsigned long long timeoutTime;
	unsigned long long losevaildtime;
};

class CGetAllSessionInfoOutput
{
public:
	explicit CGetAllSessionInfoOutput()
		: id(0), sessionId(""), timeoutTime(0), losevaildtime(0) {}
	explicit CGetAllSessionInfoOutput(const int &id, const std::string &sessionId, const unsigned long long &timeoutTime, const unsigned long long &losevaildtime)
		: id(id), sessionId(sessionId), timeoutTime(timeoutTime), losevaildtime(losevaildtime) {}
	virtual ~CGetAllSessionInfoOutput() {}

public:
	void setId(const int &id) { this->id = id; }
	const int &getId() const { return this->id; }
	void setSessionId(const std::string &sessionId) { this->sessionId = sessionId; }
	const std::string &getSessionId() const { return this->sessionId; }
	void setTimeoutTime(const unsigned long long &timeoutTime) { this->timeoutTime = timeoutTime; }
	const unsigned long long &getTimeoutTime() const { return this->timeoutTime; }
	void setLosevaildtime(const unsigned long long &losevaildtime) { this->losevaildtime = losevaildtime; }
	const unsigned long long &getLosevaildtime() const { return this->losevaildtime; }

private:
	int id;
	std::string sessionId;
	unsigned long long timeoutTime;
	unsigned long long losevaildtime;
};

}

#endif // __SESSIONMGR_DB_DB_PARAM_H__
