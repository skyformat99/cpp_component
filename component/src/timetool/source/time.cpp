#include "timetool/time.h"
#include <chrono>

namespace timetool
{

CTime::CTime()
{
}

CTime::~CTime()
{
}

int64_t CTime::nowMS() const
{
	auto time_now = std::chrono::system_clock::now();
	auto duration_in_ms = std::chrono::duration_cast<std::chrono::milliseconds>(time_now.time_since_epoch());
	return duration_in_ms.count();
}

int64_t CTime::nowNS() const
{
	auto time_now = std::chrono::system_clock::now();
	auto duration_in_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(time_now.time_since_epoch());
	return duration_in_ns.count();
}

}
