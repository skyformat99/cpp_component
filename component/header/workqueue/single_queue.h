#ifndef __SINGLE_QUEUE_EE160B028C6142D2B99A1C3F68195857_H__
#define __SINGLE_QUEUE_EE160B028C6142D2B99A1C3F68195857_H__

#include "commuchannel/commuchannel.h"
#include <list>

namespace workqueue
{

template<typename T>
class CSingleQueue
{
public:
	explicit CSingleQueue(int max = 100)
		: m_channel(max)
	{
	}

	virtual ~CSingleQueue()
	{
	}

public:
	void push(const T &item)
	{
		m_channel.write(item);
	}

	void pop(T &item)
	{
		m_channel.read(item);
	}

private:
	commuchannel::CChannel<T> m_channel;
};

}

#endif // __SINGLE_QUEUE_EE160B028C6142D2B99A1C3F68195857_H__
