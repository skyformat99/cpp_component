#ifndef __SESSION_MGR_H__
#define __SESSION_MGR_H__

#include <stdint.h>
#include <mutex>
#include <string>

namespace session
{

const static int storage_medium_memory = 1;
const static int storage_medium_sqlite = 2;
const static int storage_medium_mysql = 3;

class CSessionHandler;
class CSessionMgr
{
public:
	explicit CSessionMgr(int storageMedium = storage_medium_sqlite);
	virtual ~CSessionMgr();

public:
	bool createSession(uint64_t timeout /*ms*/, std::string &sessionId);
	bool destroySession(const std::string &sessionId);
	bool sessionIsVaild(const std::string &sessionId, bool &isVaild);
	bool resetLoseVaildTime(const std::string &sessionId);

private:
	int m_storageMedium;
	CSessionHandler *m_handler;
};

}

#endif // __SESSION_MGR_H__
