#ifndef __SESSIONMGR_DB_DB_HANDLER_H__
#define __SESSIONMGR_DB_DB_HANDLER_H__

#include <stdint.h>
#include <mutex>
#include "sessionmgr_db_param.h"

struct sqlite3;
namespace sessionmgr_db
{

class CDbHandler
{
public:
	explicit CDbHandler(const std::string &dbpath, bool isMemory = false);
	virtual ~CDbHandler();

public:
	/*@@start@@*/
	// 创建session
	uint32_t createSession(const CCreateSessionInput &input);
	// 销毁session
	uint32_t destroySession(const CDestroySessionInput &input);
	// 获取sessionId的个数
	uint32_t getSessionIdCount(const CGetSessionIdCountInput &input, CGetSessionIdCountOutput &output);
	// 更新失效时间
	uint32_t updateLoseVaildTime(const CUpdateLoseVaildTimeInput &input);
	// 获取session信息
	uint32_t getSessionInfo(const CGetSessionInfoInput &input, CGetSessionInfoOutput &output);
	// 获取全部的session信息
	uint32_t getAllSessionInfo(std::list<CGetAllSessionInfoOutput> &output);

private:
	sqlite3 *m_db;
	std::mutex m_mutex;
};

}

#endif // __SESSIONMGR_DB_DB_HANDLER_H__
