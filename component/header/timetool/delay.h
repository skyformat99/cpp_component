#ifndef __DELAY_B810C2E507FA4AABAA8B351C78BF6E89_H__
#define __DELAY_B810C2E507FA4AABAA8B351C78BF6E89_H__

#include <stdint.h>
#include <thread>
#include <mutex>
#include <functional>

namespace timetool
{

class CDelay
{
public:
	explicit CDelay();
	virtual ~CDelay();

public:
	void setTime(const uint64_t &us, std::function<void()> fn);
	void start();
	void stop();

private:
	bool m_isStop;
	bool m_isEnd;
	std::once_flag m_onceFlag;
	std::thread m_thread;
};

}

#endif // __DELAY_B810C2E507FA4AABAA8B351C78BF6E89_H__
