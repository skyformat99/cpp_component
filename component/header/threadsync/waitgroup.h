#ifndef __THREADSYNC_WAITGROUP_H__
#define __THREADSYNC_WAITGROUP_H__

#include <condition_variable>
#include <mutex>

namespace threadsync
{

class CWaitSync
{
public:
	explicit CWaitSync();
	virtual ~CWaitSync();

public:
	void add(int count = 1);
	void done();
	void wait();

private:
	int m_count;
	std::mutex m_mutex;
	std::condition_variable m_cv;
};

}

#endif
