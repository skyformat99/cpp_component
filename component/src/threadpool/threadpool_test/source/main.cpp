#include "threadpool/threadpool.h"
#include <iostream>
#include <thread>

class CHandle : public threadpool::IThreadHandle
{
public:
	explicit CHandle() {}
	virtual ~CHandle() {}

protected:
	virtual uint32_t handle()
	{
		std::cout << "handle start ..." << std::endl;
		// std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		std::cout << "handle end ..." << std::endl;
		return 0;
	}
};

int main()
{
#if 1
	threadpool::CFuncThreadPool pool(10);
	uint32_t ret = 0;
	std::thread([&ret, &pool] {
		for (int i = 0; i < 100; ++i) {
			pool.run([ret] {
				std::cout << "run start..." << std::endl;
				std::this_thread::sleep_for(std::chrono::milliseconds(1000));
				// std::cout << "run end..." << std::endl;
				return ret;
			});
		}
	}).detach();
#endif
#if 0
	threadpool::CClassHandleThreadPool pool(20);
	std::thread([&pool] {
		for (int i = 0; i < 10; ++i) {
			CHandle *handle = new CHandle;
			pool.run(handle);
		}
	}).detach();
#endif

	std::system("pause");

	return 0;
}
