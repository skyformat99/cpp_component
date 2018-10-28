#ifndef __THREADPOOL_THREAD_MGR_H__
#define __THREADPOOL_THREAD_MGR_H__

#include <stdint.h>
#include <list>
#include <mutex>
#include <functional>
#include <condition_variable>

namespace threadpool
{

class IThreadHandle;
class CThread;
class CThreadMgr
{
public:
	explicit CThreadMgr(int max = 100, int min = -1);
	virtual ~CThreadMgr();

public:
	void run(const std::function<uint32_t(void)> &fun);
	void run(IThreadHandle *handle);

protected:
	virtual CThread *createThread() = 0;
	CThread *work();

	friend class CThread;
private:
	void runEnd(CThread *thread);
	void gcLoop();

private:
	std::list<CThread*> m_queue;
	std::mutex m_queueMutex;
	std::condition_variable m_cv;
	std::mutex m_mutex;
	int m_busyCount;
	int m_count;
	bool m_isStop;
	int m_max;
	int m_min;
};

class CFuncThreadMgr : public CThreadMgr
{
public:
	explicit CFuncThreadMgr(int max = 100, int min = -1);
	virtual ~CFuncThreadMgr();

protected:
	virtual CThread *createThread();

public:
	void run(const std::function<uint32_t(void)> &fun);
};

class CClassHandleThreadMgr : public CThreadMgr
{
public:
	explicit CClassHandleThreadMgr(int max = 100, int min = -1);
	virtual ~CClassHandleThreadMgr();

protected:
	virtual CThread *createThread();

public:
	void run(IThreadHandle *handle);
};

}

#endif // __THREADPOOL_THREAD_MGR_H__
