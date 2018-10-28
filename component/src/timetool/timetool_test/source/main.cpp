#include "timetool/timer.h"
#include "timetool/delay.h"
#include "commuchannel/commuchannel.h"
#include <iostream>
#include <thread>

static commuchannel::CChannel<bool> g_channel;

void timerTest()
{
	timetool::CTimer timer;
	timer.startTimer(1000 * 3, []() -> bool {
		std::this_thread::sleep_for(std::chrono::milliseconds(1000 * 10));
		std::cout << "execute..." << std::endl;
		return true;
	}, false, timetool::CTimer::timer_mode_nowait, timetool::CTimer::thread_mode_no);
	bool result = false;
	g_channel.read(result);
}

void delayTest()
{
	timetool::CDelay delay;
	delay.setTime(1000 * 1000 * 10, [] {
		std::cout << "10s ..." << std::endl;
	});
	delay.start();
	bool result = false;
	g_channel.read(result);
}

int main()
{
	// timerTest();
	delayTest();

#ifdef WIN32
	std::system("pause");
#endif

	return 0;
}
