#ifndef __THREADPOOL_THREAD_H__
#define __THREADPOOL_THREAD_H__

#include <stdint.h>
#include <functional>
#include <condition_variable>
#include <mutex>

namespace std {
class thread;
}

namespace threadpool
{

class IThreadHandle;
class CThreadMgr;
class CThread
{
public:
	explicit CThread(CThreadMgr *mgr);
	virtual ~CThread();

protected:
	virtual void runAfter();
	virtual void handle() = 0;

private:
	void runThread();
	void stop();

private:
	CThreadMgr *m_mgr;
	std::thread *m_thread;
	bool m_isLoop;
	std::condition_variable m_cv;
	std::mutex m_mutex;
};

class CFuncThread : public CThread
{
public:
	explicit CFuncThread(CThreadMgr *mgr);
	virtual ~CFuncThread();

public:
	void run(const std::function<uint32_t(void)> &fun);

protected:
	virtual void handle();

private:
	std::function<uint32_t(void)> m_func;
};

class CClassHandleThread : public CThread
{
public:
	explicit CClassHandleThread(CThreadMgr *mgr);
	virtual ~CClassHandleThread();

public:
	void run(IThreadHandle *handle);

protected:
	virtual void handle();

private:
	IThreadHandle *m_handle;
};

}

#endif
