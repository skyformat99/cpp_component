#ifndef __COMMU_CHANNEL_37A9A597688F432096285A9772BB8243_H__
#define __COMMU_CHANNEL_37A9A597688F432096285A9772BB8243_H__

#include <queue>
#include <future>
#include <mutex>
#include <condition_variable>
#include <stdint.h>

namespace commuchannel
{

template<typename T>
class CChannel
{
public:
	explicit CChannel(int size = 1)
		: m_size(size)
		, m_queue()
		, m_mutex()
		, m_cv()
	{
	}
	virtual ~CChannel()
	{
	}

public:
	void write(const T &data)
	{
		m_mutex.lock();
		m_queue.push(data);
		m_mutex.unlock();
		std::unique_lock<std::mutex> lk(m_mutex);
		while (m_queue.size() > m_size) {
			m_cv.wait(lk);
		}
		lk.unlock();
		m_cv.notify_one();
	}
	void read(T &data)
	{
		std::unique_lock<std::mutex> lk(m_mutex);
		while (m_queue.empty()) {
			m_cv.wait(lk);
		}
		lk.unlock();
		m_mutex.lock();
		data = m_queue.front();
		m_queue.pop();
		m_mutex.unlock();
		m_cv.notify_one();
	}
	bool read(T &data, const uint32_t &timeoutMS)
	{
		std::unique_lock<std::mutex> lk(m_mutex);
		while (m_queue.empty()) {
			const std::cv_status &status = m_cv.wait_for(lk, std::chrono::milliseconds(timeoutMS));
			if (status == std::cv_status::timeout) {
				lk.unlock();
				return false;
			}
		}
		lk.unlock();
		m_mutex.lock();
		data = m_queue.front();
		m_queue.pop();
		m_mutex.unlock();
		m_cv.notify_one();
		return true;
	}

private:
	int m_size;
	std::queue<T> m_queue;
	std::mutex m_mutex;
	std::condition_variable m_cv;
};

}

#endif // __COMMU_CHANNEL_37A9A597688F432096285A9772BB8243_H__
