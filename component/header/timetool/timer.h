#ifndef __TIMER_287739CD860844E9BBCBDF37B712F5C7_H__
#define __TIMER_287739CD860844E9BBCBDF37B712F5C7_H__

#include <functional>
#include <stdint.h>
#include <vector>
#include <thread>
#include <mutex>

namespace timetool
{

class CTimer
{
public:
	enum timer_mode
	{
		// reclocking util callback return
		timer_mode_wait = 1,
		// reclocking before callback return
		timer_mode_nowait = 2
	};
	enum thread_mode
	{
		thread_mode_no = 1,
		thread_mode_detach = 2,
		thread_mode_join = 3
	};

public:
	explicit CTimer();
	virtual ~CTimer();

public:
	// if callback return true will continue, if return false will end
	void startTimer(
		const int64_t &timerTimeoutMS
		, std::function<bool()> callback
		, const bool &firstIsExecute = false
		, const timer_mode &mode = timer_mode_nowait
		, const thread_mode &threadmode = thread_mode_no
		, const int64_t &checkIntervalUS = 10000
		, const int64_t &maxTimeoutMS = -1);

private:
	class CItem
	{
	public:
		explicit CItem();
		virtual ~CItem();

	public:
		void startTimer(
			const int64_t &timerTimeoutMS
			, std::function<bool()> callback
			, const bool &firstIsExecute = false
			, const timer_mode &mode = timer_mode_nowait
			, const thread_mode &threadmode = thread_mode_no
			, const int64_t &checkIntervalUS = 10000
			, const int64_t &maxTimeoutMS = -1);

	private:
		bool m_isEnd;
		std::thread m_thread;
	};

private:
	std::vector<CItem*> m_items;
	std::mutex m_itemsMutex;
};

}

#endif // __TIMER_287739CD860844E9BBCBDF37B712F5C7_H__
