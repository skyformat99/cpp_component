#include <thread>
#include "thread.h"
#include "thread_mgr.h"
#include "threadpool/threadpool.h"

namespace threadpool
{

CThread::CThread(CThreadMgr *mgr)
	: m_mgr(mgr)
	, m_thread(nullptr)
	, m_isLoop(true)
	, m_cv()
	, m_mutex()
{
	m_thread = new std::thread(&CThread::runThread, this);
}

CThread::~CThread()
{
	stop();
	m_thread->join();
	if (m_thread != nullptr) {
		delete m_thread;
		m_thread = nullptr;
	}
}

void CThread::runThread()
{
	while (m_isLoop)
	{
		std::unique_lock<std::mutex> lk(m_mutex);
		m_cv.wait(lk);
		if (m_isLoop == false) {
			break;
		}
		this->handle();
		m_mgr->runEnd(this);
	}
}

void CThread::runAfter()
{
	m_cv.notify_one();
}

void CThread::stop()
{
	m_isLoop = false;
	m_cv.notify_one();
}

///////////////////////////////////////
CFuncThread::CFuncThread(CThreadMgr *mgr)
	: CThread(mgr)
	, m_func()
{
}

CFuncThread::~CFuncThread()
{
}

void CFuncThread::handle()
{
	m_func();
}

void CFuncThread::run(const std::function<uint32_t(void)> &fun)
{
	m_func = fun;
	CThread::runAfter();
}

///////////////////////////////////////
CClassHandleThread::CClassHandleThread(CThreadMgr *mgr)
	: CThread(mgr)
	, m_handle(nullptr)
{
}

CClassHandleThread::~CClassHandleThread()
{
}

void CClassHandleThread::handle()
{
	m_handle->handle();
}

void CClassHandleThread::run(IThreadHandle *handle)
{
	m_handle = handle;
	CThread::runAfter();
}

}
