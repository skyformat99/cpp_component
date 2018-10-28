#include <exception>
#include <thread>
#include "thread.h"
#include "thread_mgr.h"
#include "threadpool/threadpool.h"

namespace threadpool
{

CThreadMgr::CThreadMgr(int max /* = 100 */, int min /* = -1 */)
	: m_queue()
	, m_queueMutex()
	, m_cv()
	, m_mutex()
	, m_busyCount(0)
	, m_count(0)
	, m_isStop(false)
	, m_max(max)
	, m_min(min)
{
	gcLoop();
}

CThreadMgr::~CThreadMgr()
{
	m_queueMutex.lock();
	for (auto thread : m_queue) {
		delete thread;
		thread = nullptr;
	}
	m_queue.clear();
	m_busyCount = 0;
	m_queueMutex.unlock();
	m_isStop = true;
}

CThread *CThreadMgr::work()
{
	CThread *thread = nullptr;
	do 
	{
#if 0
		m_mutex.lock();
		if (!m_queue.empty()) {
			thread = m_queue.front();
			m_queue.pop_front();
		}
		m_mutex.unlock();
		if (thread == nullptr && m_count < m_max) {
			thread = createThread();
			++m_count;
		}
		if (thread == nullptr) {
			std::unique_lock<std::mutex> lk(m_mutex);
			m_cv.wait(lk);
		}
#else
		m_queueMutex.lock();
		size_t size = m_queue.size();
		if (size == 0 && m_busyCount < m_max) {
			thread = createThread();
			++m_busyCount;
			m_queueMutex.unlock();
			break;
		}
		if (!m_queue.empty()) {
			thread = m_queue.front();
			m_queue.pop_front();
			++m_busyCount;
			m_queueMutex.unlock();
			break;
		}
		m_queueMutex.unlock();
		if (m_busyCount >= m_max) {
			std::unique_lock<std::mutex> lk(m_mutex);
			m_cv.wait(lk);
		}
#endif
	} while (!thread);
	return thread;
}

void CThreadMgr::runEnd(CThread *thread)
{
	m_queueMutex.lock();
	--m_busyCount;
	m_queue.push_back(thread);
	m_queueMutex.unlock();
	m_cv.notify_all();
}

void CThreadMgr::gcLoop()
{
	if (m_min != -1) {
		std::thread([this] {
			while (!m_isStop) {
				int size = (int)m_queue.size();
				if (size > m_min) {
					int clearCount = size - m_min;
					m_queueMutex.lock();
					for (int i = 0; i < clearCount; ++i) {
						CThread *thread = m_queue.front();
						if (thread != nullptr) {
							delete thread;
							thread = nullptr;
						}
						m_queue.pop_front();
					}
					m_queueMutex.unlock();
				}
				else {
					std::this_thread::sleep_for(std::chrono::seconds(30));
				}
			}
		}).detach();
	}
}

////////////////////////////////
CFuncThreadMgr::CFuncThreadMgr(int max /* = 100 */, int min /* = -1 */)
	: CThreadMgr(max, min)
{
}

CFuncThreadMgr::~CFuncThreadMgr()
{
}

CThread *CFuncThreadMgr::createThread()
{
	return new CFuncThread(this);
}

void CFuncThreadMgr::run(const std::function<uint32_t(void)> &fun)
{
	CFuncThread *thread = dynamic_cast<CFuncThread*>(work());
	thread->run(fun);
}


////////////////////////////////
CClassHandleThreadMgr::CClassHandleThreadMgr(int max /* = 100 */, int min /* = -1 */)
	: CThreadMgr(max, min)
{
}

CClassHandleThreadMgr::~CClassHandleThreadMgr()
{
}

CThread *CClassHandleThreadMgr::createThread()
{
	return new CClassHandleThread(this);
}

void CClassHandleThreadMgr::run(IThreadHandle *handle)
{
	CClassHandleThread *thread = dynamic_cast<CClassHandleThread*>(work());
	thread->run(handle);
}

}
