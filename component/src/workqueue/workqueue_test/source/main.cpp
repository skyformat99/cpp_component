#include <iostream>
#include <thread>
#include "workqueue/single_queue.h"
#include "commuchannel/commuchannel.h"

void singleQueueTest()
{
	commuchannel::CChannel<bool> channel;
	workqueue::CSingleQueue<int> queue;
	std::thread([&queue] {
		int i = 0;
		while (1) {
			queue.push(i++);
		}
	}).detach();
	std::thread([&queue] {
		while (1) {
			int i = 0;
			queue.pop(i);
			std::cout << i << std::endl;
		}
	}).detach();
	bool result = false;
	channel.read(result);
}

int main()
{
	singleQueueTest();

#ifdef WIN32
	std::system("pause");
#endif

	return 0;
}
