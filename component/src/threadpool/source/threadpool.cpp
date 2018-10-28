#include "threadpool/threadpool.h"
#include "thread_mgr.h"

namespace threadpool
{

CFuncThreadPool::CFuncThreadPool(int max /* = 100 */, int min /* = -1 */)
	: m_mgr(nullptr)
{
	if (max < min) {
		throw "[count error] max < min";
	}
	m_mgr = new CFuncThreadMgr(max, min);
}

CFuncThreadPool::~CFuncThreadPool()
{
	if (m_mgr != nullptr) {
		delete m_mgr;
		m_mgr = nullptr;
	}
}

void CFuncThreadPool::run(const std::function<uint32_t(void)> &fun)
{
	m_mgr->run(fun);
}

//////////////////////////////////////////////
CClassHandleThreadPool::CClassHandleThreadPool(int max /* = 100 */, int min /* = -1 */)
	: m_mgr(nullptr)
{
	if (max < min) {
		throw "[count error] max < min";
	}
	m_mgr = new CClassHandleThreadMgr(max, min);
}

CClassHandleThreadPool::~CClassHandleThreadPool()
{
	if (m_mgr != nullptr) {
		delete m_mgr;
		m_mgr = nullptr;
	}
}

void CClassHandleThreadPool::run(IThreadHandle *handle)
{
	m_mgr->run(handle);
}

}
