#include "sessionmgr/sessionmgr.h"
#include "sessionhandler.h"

#define check_null(p) \
	if (p == nullptr) return false;

namespace session
{

CSessionMgr::CSessionMgr(int storageMedium /* = storage_medium_sqlite */)
	: m_storageMedium(storageMedium)
	, m_handler(nullptr)
{
	switch (m_storageMedium)
	{
	case storage_medium_sqlite:
		m_handler = new CSqliteSessionHandler();
		break;
	case storage_medium_memory:
		m_handler = new CMemorySessionHandler();
		break;
	case storage_medium_mysql:
		m_handler = new CMysqlSessionHandler();
		break;
	default:
		break;
	}
}

CSessionMgr::~CSessionMgr()
{
	if (m_handler != nullptr) {
		delete m_handler;
		m_handler = nullptr;
	}
}

bool CSessionMgr::createSession(uint64_t timeout, std::string &sessionId)
{
	check_null(m_handler);
	return m_handler->createSession(timeout, sessionId);
}

bool CSessionMgr::destroySession(const std::string &sessionId)
{
	check_null(m_handler);
	return m_handler->destroySession(sessionId);
}

bool CSessionMgr::sessionIsVaild(const std::string &sessionId, bool &isVaild)
{
	check_null(m_handler);
	return m_handler->sessionIsVaild(sessionId, isVaild);
}

bool CSessionMgr::resetLoseVaildTime(const std::string &sessionId)
{
	check_null(m_handler);
	return m_handler->resetLoseVaildTime(sessionId);
}

}
