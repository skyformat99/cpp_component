#include "commuchannel/commuchannel.h"
#include <iostream>
#include <thread>
#include <string>
#include <regex>

void channelTest()
{
	std::cout << "help:\
	q: exit\n\
	c: read data from channel\n\
	p: write data to channel, param: data, ex: p hello\n" << std::endl;

	commuchannel::CChannel<std::string> channel;
	std::string in;
	while (1)
	{
		getline(std::cin, in);
		if (in == "q") {
			break;
		}
		else if (in == "c") {
			std::string data("");
			channel.read(data);
			std::cout << "read data: " << data << std::endl;
		}
		else {
			std::regex r("p (.*?)");
			std::smatch result;
			bool b = std::regex_match(in, result, r);
			if (!b) return;
			if (result.empty()) return;
			channel.write(result[1]);
		}
	}
}

void channelProductCustomerTest()
{
	static long long index = 0;
	commuchannel::CChannel<std::string> channel(1);
	std::thread([&channel] {
		while (1) {
			std::string data("");
			channel.read(data);
			std::cout << data << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
	}).detach();
	std::thread([&channel] {
		while (1) {
			std::string data("test data: " + std::to_string(index++));
			channel.write(data);
			std::cout << "send: " << std::to_string(index) << std::endl;
		}
	}).detach();
	
	while (1) {
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

void channelProductCustomerTest2()
{
	static long long index = 0;
	commuchannel::CChannel<std::string> channel(1);
	std::thread([&channel] {
		while (1) {
			std::string data("");
			channel.read(data, 5);
			std::cout << data << std::endl;
		}
	}).detach();
	std::thread([&channel] {
		while (1) {
			std::string data("test data: " + std::to_string(index++));
			channel.write(data);
			std::cout << "send: " << std::to_string(index) << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
	}).detach();

	while (1) {
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

int main()
{
	// channelTest();
	// channelProductCustomerTest();
	channelProductCustomerTest2();

	std::system("pause");

	return 0;
}
