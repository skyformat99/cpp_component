#include "sessionhandler.h"
#include "randtool/uuid.h"
#include "sessionmgr/sessionmgr.h"
#include "sessionmgr_db_param.h"
#include "sessionmgr_db_handler.h"

namespace session
{

static randtool::CUuid g_uuid;

CSessionHandler::CSessionHandler()
	: m_isRun(true)
	, m_timerThread()
	, m_sessions()
	, m_mutex()
{
	m_timerThread = std::thread([this] {
		while (m_isRun) {
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			handleSession();
		}
	});
}

CSessionHandler::~CSessionHandler()
{
	m_isRun = false;
	if (m_timerThread.joinable()) {
		m_timerThread.join();
	}
}

void CSessionHandler::init()
{
	loadSessions();
}

bool CSessionHandler::createSession(uint64_t timeout, std::string &sessionId)
{
	insertSessionToMemory(sessionId, timeout);
	return true;
}

bool CSessionHandler::destroySession(const std::string &sessionId)
{
	return deleteSessionFromMemory(sessionId);
}

bool CSessionHandler::resetLoseVaildTime(const std::string &sessionId)
{
	TSessionInfo info;
	bool ret = getSessionInfoFromMemory(sessionId, info);
	if (ret == false) return ret;
	return updateLoseVaildTimeToMemory(sessionId, info.timeoutTime + getNowTimeStamp());
}

bool CSessionHandler::sessionIsVaild(const std::string &sessionId, bool &isVaild)
{
	auto iter = m_sessions.find(sessionId);
	if (iter == m_sessions.end()) {
		isVaild = false;
		return false;
	}
	isVaild = true;
	return true;
}

void CSessionHandler::loadSessions()
{
	m_mutex.lock();
	this->sessions(m_sessions);
	m_mutex.unlock();
}

void CSessionHandler::handleSession()
{
	for (auto iter = m_sessions.begin(); iter != m_sessions.end(); ++iter) {
		uint64_t now = getNowTimeStamp();
		if (now > iter->second.loseVaildTime) {
			destroySession(iter->second.sessionId);
			break;
		}
	}
}

void CSessionHandler::insertSessionToMemory(const std::string &sessionId, uint64_t timeout)
{
	m_mutex.lock();
	TSessionInfo info;
	info.loseVaildTime = getNowTimeStamp() + timeout;
	info.sessionId = sessionId;
	info.timeoutTime = timeout;
	m_sessions.insert(std::make_pair(sessionId, info));
	m_mutex.unlock();
}

bool CSessionHandler::updateLoseVaildTimeToMemory(const std::string &sessionId, uint64_t loseVaildTime)
{
	m_mutex.lock();
	auto iter = m_sessions.find(sessionId);
	if (iter == m_sessions.end()) {
		return false;
	}
	iter->second.loseVaildTime = loseVaildTime;
	iter->second.sessionId = sessionId;
	m_mutex.unlock();
	return true;
}

bool CSessionHandler::deleteSessionFromMemory(const std::string &sessionId)
{
	m_mutex.lock();
	auto iter = m_sessions.find(sessionId);
	if (iter == m_sessions.end()) {
		return false;
	}
	m_sessions.erase(iter);
	m_mutex.unlock();
	return true;
}

bool CSessionHandler::getSessionInfoFromMemory(const std::string &sessionId, TSessionInfo &info)
{
	auto iter = m_sessions.find(sessionId);
	if (iter == m_sessions.end()) {
		return false;
	}
	info = iter->second;
	return true;
}

uint64_t CSessionHandler::getNowTimeStamp()
{
	auto time_now = std::chrono::system_clock::now();
	auto duration_in_ms = std::chrono::duration_cast<std::chrono::milliseconds>(time_now.time_since_epoch());
	return duration_in_ms.count();
}

//////////////////////////////////////////
CSqliteSessionHandler::CSqliteSessionHandler(bool isMemory /* = false */)
	: m_handler(new sessionmgr_db::CDbHandler("session.db", isMemory))
{
	CSessionHandler::init();
}

CSqliteSessionHandler::~CSqliteSessionHandler()
{
	if (m_handler != nullptr) {
		delete m_handler;
		m_handler = nullptr;
	}
}

bool CSqliteSessionHandler::createSession(uint64_t timeout, std::string &sessionId)
{
	sessionmgr_db::CCreateSessionInput input;
	input.setSessionId(sessionId = g_uuid.generator());
	input.setTimeoutTime(timeout);
	input.setLosevaildtime(getNowTimeStamp() + timeout);
	uint32_t ret = m_handler->createSession(input);
	if (ret != 0) return false;

	return CSessionHandler::createSession(timeout, sessionId);
}

bool CSqliteSessionHandler::destroySession(const std::string &sessionId)
{
	sessionmgr_db::CDestroySessionInput input;
	input.setSessionId(sessionId);
	uint32_t ret = m_handler->destroySession(input);
	if (ret != 0) return false;

	return CSessionHandler::destroySession(sessionId);
}

bool CSqliteSessionHandler::sessionIsVaild(const std::string &sessionId, bool &isVaild)
{
	sessionmgr_db::CGetSessionIdCountInput input;
	input.setSessionId(sessionId);
	sessionmgr_db::CGetSessionIdCountOutput output;
	uint32_t ret = m_handler->getSessionIdCount(input, output);
	if (ret != 0) return false;
	int count = output.getCount();
	if (count == 0) {
		isVaild = false;
	}
	else {
		isVaild = true;
	}

	return CSessionHandler::sessionIsVaild(sessionId, isVaild);
}

bool CSqliteSessionHandler::resetLoseVaildTime(const std::string &sessionId)
{
	sessionmgr_db::CGetSessionInfoInput getSessionInfoInput;
	getSessionInfoInput.setSessionId(sessionId);
	sessionmgr_db::CGetSessionInfoOutput getSessionInfoOutput;
	uint32_t ret = m_handler->getSessionInfo(getSessionInfoInput, getSessionInfoOutput);
	if (ret != 0) return false;
	sessionmgr_db::CUpdateLoseVaildTimeInput input;
	input.setSessionId(sessionId);
	input.setLosevaildtime(getNowTimeStamp() + getSessionInfoOutput.getTimeoutTime());
	ret = m_handler->updateLoseVaildTime(input);
	if (ret != 0) return false;

	return CSessionHandler::resetLoseVaildTime(sessionId);
}

void CSqliteSessionHandler::sessions(std::map<std::string, TSessionInfo> &sessionInfos)
{
	std::list<sessionmgr_db::CGetAllSessionInfoOutput> output;
	uint32_t ret = m_handler->getAllSessionInfo(output);
	if (ret != 0) return;
	sessionInfos.clear();
	for (auto iter = output.begin(); iter != output.end(); ++iter) {
		TSessionInfo info;
		info.loseVaildTime = iter->getLosevaildtime();
		info.sessionId = iter->getSessionId();
		info.timeoutTime = iter->getTimeoutTime();
		sessionInfos.insert(std::make_pair(iter->getSessionId(), info));
	}
}

//////////////////////////////////////////
CMemorySessionHandler::CMemorySessionHandler()
	: CSqliteSessionHandler(true)
{
}

CMemorySessionHandler::~CMemorySessionHandler()
{
}

//////////////////////////////////////////
CMysqlSessionHandler::CMysqlSessionHandler()
{
	CSessionHandler::init();
}

CMysqlSessionHandler::~CMysqlSessionHandler()
{
}

bool CMysqlSessionHandler::createSession(uint64_t timeout, std::string &sessionId)
{
	return true;
}

bool CMysqlSessionHandler::destroySession(const std::string &sessionId)
{
	return true;
}

bool CMysqlSessionHandler::sessionIsVaild(const std::string &sessionId, bool &isVaild)
{
	return true;
}

bool CMysqlSessionHandler::resetLoseVaildTime(const std::string &sessionId)
{
	return true;
}

void CMysqlSessionHandler::sessions(std::map<std::string, TSessionInfo> &sessionInfos)
{
}

}
