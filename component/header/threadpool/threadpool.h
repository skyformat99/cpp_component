#ifndef __THREADPOOL_THREAD_POOL_H__
#define __THREADPOOL_THREAD_POOL_H__

#include <stdint.h>
#include <functional>

namespace threadpool
{

class CFuncThreadMgr;
class CClassHandleThreadMgr;
class IThreadHandle
{
public:
	virtual uint32_t handle() = 0;
};

class CFuncThreadPool
{
public:
	explicit CFuncThreadPool(int max = 100, int min = -1);
	virtual ~CFuncThreadPool();

public:
	void run(const std::function<uint32_t(void)> &fun);

private:
	CFuncThreadMgr *m_mgr;
};

class CClassHandleThreadPool
{
public:
	explicit CClassHandleThreadPool(int max = 100, int min = -1);
	virtual ~CClassHandleThreadPool();

public:
	void run(IThreadHandle *handle);

private:
	CClassHandleThreadMgr *m_mgr;
};

}

#endif // __THREADPOOL_THREAD_POOL_H__
