#include <iostream>
#include <string>
#include <map>
#include <thread>
#include "ws_client/ws_client.h"
#include "ws_common/ws_msg_parser.h"
#include "ws_common/ws_const.h"

// #define SELECT
#define REGISTER

void printHelp()
{
	std::cout << "help: \n\
		input a: subscribe /notification/access/type/+\n\
		input b: subscribe /notification/+/type/alarm\n\
		input c: subscribe /notification/+/type/+\n\
		input s1: send /notification/access/type/status broadcast\n\
		input s2: send /notification/access/type/alarm broadcast\n\
		input s3: send /notification/face/type/status broadcast\n\
		input s4: send /notification/face/type/alarm broadcast\n\
		input q: quit" << std::endl;
}

void select(websocket::CClient *client)
{
	std::string input("");
	while (1)
	{
		std::cin >> input;
		if (input == "q") {
			std::cout << "bye" << std::endl;
			break;
		}
		else if (input == "a") {
			std::map<std::string, std::string> headers;
			headers.insert(std::make_pair("topic", "/notification/access/type/+"));
			client->post("/subscribe", "1.0", nullptr, &headers, "", nullptr);
		}
		else if (input == "b") {
			std::map<std::string, std::string> headers;
			headers.insert(std::make_pair("topic", "/notification/+/type/alarm"));
			client->post("/subscribe", "1.0", nullptr, &headers, "", nullptr);
		}
		else if (input == "c") {
			std::map<std::string, std::string> headers;
			headers.insert(std::make_pair("topic", "/notification/+/type/+"));
			client->post("/subscribe", "1.0", nullptr, &headers, "", nullptr);
		}
		else if (input == "s1") {
			std::map<std::string, std::string> headers;
			headers.insert(std::make_pair("topic", "/notification/access/type/status"));
			client->asyncSend(websocket::method_post, "/broadcast", "1.0", nullptr, &headers, "");
		}
		else if (input == "s2") {
			std::map<std::string, std::string> headers;
			headers.insert(std::make_pair("topic", "/notification/access/type/alarm"));
			client->asyncSend(websocket::method_post, "/broadcast", "1.0", nullptr, &headers, "");
		}
		else if (input == "s3") {
			std::map<std::string, std::string> headers;
			headers.insert(std::make_pair("topic", "/notification/face/type/status"));
			client->asyncSend(websocket::method_post, "/broadcast", "1.0", nullptr, &headers, "");
		}
		else if (input == "s4") {
			std::map<std::string, std::string> headers;
			headers.insert(std::make_pair("topic", "/notification/face/type/alarm"));
			client->asyncSend(websocket::method_post, "/broadcast", "1.0", nullptr, &headers, "");
		}
	}
}

int main()
{
#if 1
	websocket::CClient client("ws://192.168.9.15:9002", 5 * 1000);
	if (!client) {
		std::cout << "connect error" << std::endl;
		// return -1;
	}
	client.setMessageCallback([](const std::string &msg) {
		websocket::CCustomMsgParser parser;
		parser.parse(msg);
		std::cout << "body: " << parser.body() << std::endl;
	});
#endif
#ifdef SELECT
	printHelp();
	select(&client);
#elif defined REGISTER
	client.registerNtfCallback("/notification/access/type/status/"
		, [](const websocket::CRequest &request, websocket::CResponse &response, websocket::CClient *cli) {
		std::cout << "recv /notification/access/type/status notify" << std::endl;
	});
	printHelp();
	select(&client);
#else
	std::map < std::string, std::string> params;
	std::map<std::string, std::string> headers;
	headers.insert(std::make_pair("topic", "/notification/access/type/+"));
	std::system("pause");
	std::thread([&client, params, headers] {
		std::string response;
		// client.post("ws://192.168.9.15:9002/test/uuid/abc/id/123", params, headers, "client: test", response);
		client.post("/subscribe", "1.0", &params, &headers, "subscribe /notification/access/type/+", &response);
		std::cout << "recv response body: " << response << std::endl;
	}).detach();

	std::system("pause");
#endif

	return 0;
}
