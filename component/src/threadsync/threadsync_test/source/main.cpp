#include "threadsync/waitgroup.h"
#include <string>
#include <iostream>
#include <thread>

void waitgroup_test()
{
	threadsync::CWaitSync waitSync;
	for (int i = 0; i < 100; ++i) {
		waitSync.add();
		std::cout << std::to_string(i) + " thread start" << std::endl;
		std::thread([&waitSync] {
			std::this_thread::sleep_for(std::chrono::seconds(10));
			waitSync.done();
		}).detach();
	}
	waitSync.wait();
	std::cout << "all thread execute complate" << std::endl;
}

int main()
{
	waitgroup_test();

	std::system("pause");

	return 0;
}
