#include "timetool/delay.h"

namespace timetool
{

CDelay::CDelay()
	: m_isStop(true)
	, m_isEnd(false)
	, m_thread()
	, m_onceFlag()
{
}

CDelay::~CDelay()
{
	m_isEnd = true;
	if (m_thread.joinable()) {
		m_thread.join();
	}
}

void CDelay::setTime(const uint64_t &us, std::function<void()> fn)
{
	std::call_once(m_onceFlag, [this, us, fn] {
		m_thread = std::thread([this, us, fn] {
			while (!m_isEnd) {
				if (!m_isStop) {
					fn();
					std::this_thread::sleep_for(std::chrono::microseconds(us));
				}
				else {
					std::this_thread::sleep_for(std::chrono::milliseconds(1000));
				}
			}
		});
	});
}

void CDelay::start()
{
	m_isStop = false;
}

void CDelay::stop()
{
	m_isStop = true;
}

}
