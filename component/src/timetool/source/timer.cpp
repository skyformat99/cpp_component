#include "timetool/time.h"
#include "timetool/timer.h"

namespace timetool
{

CTimer::CTimer()
	: m_items()
	, m_itemsMutex()
{
}

CTimer::~CTimer()
{
	while (!m_items.empty()) {
		CItem *ptr = m_items.back();
		if (ptr != nullptr) {
			delete ptr;
			ptr = nullptr;
		}
		m_items.pop_back();
	}
}

void CTimer::startTimer(const int64_t &timerTimeoutMS , std::function<bool()> callback , const bool &firstIsExecute /* = false  */, const timer_mode &mode /* = timer_mode_nowait  */, const thread_mode &threadmode /* = thread_mode_no  */, const int64_t &checkIntervalUS /* = 10000  */, const int64_t &maxTimeoutMS /* = -1 */)
{
	CItem *item = new CItem();
	item->startTimer(timerTimeoutMS, callback, firstIsExecute, mode, threadmode, checkIntervalUS, maxTimeoutMS);
	std::lock_guard<std::mutex> lk(m_itemsMutex);
	m_items.push_back(item);
}

CTimer::CItem::CItem()
	: m_isEnd(false)
	, m_thread()
{
}

CTimer::CItem::~CItem()
{
	m_isEnd = true;
	if (m_thread.joinable()) {
		m_thread.join();
	}
}

void CTimer::CItem::startTimer(const int64_t &timerTimeoutMS, std::function<bool()> callback, const bool &firstIsExecute /* = false  */, const timer_mode &mode /* = timer_mode_nowait  */, const thread_mode &threadmode /* = thread_mode_no  */, const int64_t &checkIntervalUS /* = 10000  */, const int64_t &maxTimeoutMS /* = -1 */)
{
	timetool::CTime time;
	m_thread = std::thread([this, time, timerTimeoutMS, firstIsExecute, callback, mode, checkIntervalUS, maxTimeoutMS] {
		bool firstIsFinish = false;
		int64_t startTime = time.nowMS();
		int64_t startConst = startTime;
		while (!m_isEnd) {
			if (firstIsExecute && !firstIsFinish) {
				if (!callback()) break;
				firstIsFinish = true;
			}
			const int64_t &now = time.nowMS();
			if (maxTimeoutMS > 0 && (now - startConst > maxTimeoutMS)) break;
			if (timerTimeoutMS <= (now - startTime)) {
				if (mode == timer_mode_nowait) {
					startTime = time.nowMS();
					if (!callback()) break;
				}
				else if (mode == timer_mode_wait) {
					if (!callback()) break;
					startTime = time.nowMS();
				}
			}
			else {
				std::this_thread::sleep_for(std::chrono::microseconds(checkIntervalUS));
			}
		}
	});
	switch (threadmode)
	{
	case thread_mode_detach:
		m_thread.detach();
		break;
	case thread_mode_join:
		m_thread.join();
		break;
	case thread_mode_no:
		break;
	default:
		break;
	}
}

}
