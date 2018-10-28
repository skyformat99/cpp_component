#include "threadsync/waitgroup.h"

namespace threadsync
{

CWaitSync::CWaitSync()
	: m_count(0)
	, m_mutex()
	, m_cv()
{
}

CWaitSync::~CWaitSync()
{
}

void CWaitSync::wait()
{
	std::unique_lock<std::mutex> lk(m_mutex);
	do 
	{
		m_cv.wait(lk);
	} while (m_count > 0);
}

void CWaitSync::add(int count /* = 1 */)
{
	m_mutex.lock();
	m_count += count;
	m_mutex.unlock();
}

void CWaitSync::done()
{
	m_mutex.lock();
	--m_count;
	m_mutex.unlock();
	m_cv.notify_one();
}

}
