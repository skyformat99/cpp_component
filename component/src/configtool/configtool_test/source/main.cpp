#include "configtool/jsonconfig.h"
#include "configtool/kvjsonconfig.h"
#include <iostream>
#include <thread>
#include <string>
#include <regex>

void jsonConfigTest()
{
	configtool::CJsonConfig jsonConfig("jsonconfig.txt");
	Json::Value value;
	value["ip"] = "127.0.0.1";
	value["port"] = 23650;
	jsonConfig.setJsonValue(value);
	Json::Value result;
	jsonConfig.getJsonValue(result);
	const std::string &ip = result["ip"].asString();
	const short &port = result["port"].asUInt();
	std::cout << "ip: " << ip << std::endl;
	std::cout << "port: " << port << std::endl;
}

void kvjsonConfigTest()
{
	configtool::CKvJsonConfig config("kvjsonconfig.txt");
	config.encoding(configtool::CKvJsonConfig::KV<std::string>("field1", "123"));
	config.encoding("main1", configtool::CKvJsonConfig::KV<int>("field2", 100));
	config.load();
	std::string field1("");
	int field2 = 0;
	config.firstString("field1", field1);
	config.secondInt("main1", "field2", field2);
	std::cout << "field1: " << field1 << std::endl;
	std::cout << "field2: " << field2 << std::endl;
}

int main()
{
	jsonConfigTest();
	// kvjsonConfigTest();

	std::system("pause");

	return 0;
}
