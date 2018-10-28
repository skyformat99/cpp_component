#include "curl_client/curl_client.h"
#include <iostream>
#include <thread>
#include <chrono>

std::chrono::time_point<std::chrono::system_clock> recordTime()
{
	return std::chrono::system_clock::now();
}

double calcTime(const std::chrono::time_point<std::chrono::system_clock> &start
	, const std::chrono::time_point<std::chrono::system_clock> &end)
{
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
	return double(duration.count()) * std::chrono::microseconds::period::num / std::chrono::microseconds::period::den;
}

void request(http::CCurlHttpClient* client)
{
#if 0
	std::string res;
	logictool::CLogicTool logictool;
	const std::string &base64 = logictool.image2base64("test.jpg");
	auto start = recordTime();
	client->post("http://192.168.9.15:8080", nullptr, base64, nullptr, &res);
	auto end = recordTime();
	std::cout << calcTime(start, end) << std::endl;
#endif
	std::string response("");
	std::map<std::string, std::string> resHeaders;
	client->get("http://192.168.9.15:8080", nullptr, nullptr, &response, resHeaders, false, 1000 * 10);
	std::cout << response << std::endl;
#if 0
	client->put("http://192.168.9.15:8080", nullptr, "hello", nullptr, &res);
	std::cout << res << std::endl;
	res.clear();
	client->del("http://192.168.9.15:8080", nullptr, nullptr, &res);
	std::cout << res << std::endl;
	res.clear();
	client->updated("http://192.168.9.15:8080", nullptr, nullptr, &res);
	std::cout << res << std::endl;
	res.clear();
	client->deleted("http://192.168.9.15:8080", nullptr, nullptr, &res);
	std::cout << res << std::endl;
#endif
}

void baseRequest()
{
	http::CCurlHttpClient client;
	int len = 100;
	request(&client);
#if 0
	while (1)
	{
		std::thread([&client] {
			request(&client);
		}).detach();
	}
#endif
}

////////////////////
void uploadTest()
{
	http::CCurlHttpClient client;
	std::string response;
	client.uploadFile("http://192.168.9.15:8080/upload"
		, "test.jpg", response);
}

////////////////////
void downloadTest()
{
	http::CCurlHttpClient client;
	std::string response;
	client.post("http://192.168.9.15:8080/start/download", nullptr, "", nullptr, &response);
	std::string base64;
	client.downloadFile("http://192.168.9.15:8080/download", response, "download/test1.jpg");
}

////////////////////
void redirTest()
{
	http::CCurlHttpClient client;
	std::string response;
	client.post("http://192.168.9.15:8080/redir/src", nullptr, "hello", nullptr, &response, nullptr, true);
}

/////////////////////
void urlParamTest()
{
	http::CCurlHttpClient client;
	std::string res;
	std::map<std::string, std::string> params;
	params.insert(std::make_pair("test", "¹þ¹þ¹þ"));
	client.get("http://192.168.9.15:8080/hello", &params, nullptr, &res);
	std::cout << res << std::endl;
}

/////////////////////
void httpsTest()
{
	http::CCurlHttpClient client;
	std::string res;
	std::map<std::string, std::string> headers;
	headers.insert(std::make_pair("Content-Type", "application/json"));
	headers.insert(std::make_pair("app_key", "TA42fbi_qLfTtivYpCXXvcxx95ka"));
	headers.insert(std::make_pair("Authorization", "bearer e1eb7e2de2d7a912a4b9a5bcc42897"));
	std::map<std::string, std::string> params;
	params.insert(std::make_pair("appId", "TA42fbi_qLfTtivYpCXXvcxx95ka"));
	params.insert(std::make_pair("pageNo", "0"));
	params.insert(std::make_pair("pageSize", "1000"));
	int code = 0;
	int ret = client.get("https://180.101.147.89:8743/iocm/app/dm/v1.1.0/devices", &params, &headers, &res, &code);
	std::cout << res << std::endl;
}

/////////////////////
void httpsTest2()
{
	http::CCurlHttpClient client;
	std::string res;
	std::string json = "{\"appId\":\"5yttNC4HZlTrYDN42ig5IddlFBsa\",\"secret\" : \"u4ZGKjcj7ozZuussEW5RrRxXRdka\",\"refreshToken\" : \"fb83802cbf5da28ec1c89e8662fa8\"}";
	int code = 0;
	int ret = client.post("https://180.101.147.89:8743/iocm/app/sec/v1.1.0/refreshToken", nullptr, json, nullptr, &res, &code);
	std::cout << res << std::endl;
}

/////////////////////////
void multipartTest()
{
	http::CCurlHttpClient client;
	std::string res;
	std::vector<http::formdata::TFormData> formData;
	http::formdata::TFormData data;
	formData.push_back(http::formdata::TFormData(http::formdata::EFORMDATA_TYPE_FILE, "file", "test.jpg"));
	formData.push_back(http::formdata::TFormData(http::formdata::EFORMDATA_TYPE_FILE, "file", "test.jpg"));
	formData.push_back(http::formdata::TFormData(http::formdata::EFORMDATA_TYPE_FILE, "file", "test.jpg"));
	int resCode = 200;
	int ret = client.post("http://192.168.9.15:8080/multi/file", nullptr, nullptr, &res, nullptr, nullptr, 0, nullptr, &formData, &resCode);
	std::cout << res << std::endl;
}

//////////////////////////
void customHttpsTest()
{
	http::CCurlHttpClient client;
	std::string res;
	std::map<std::string, std::string> resHeader;
	int ret = client.get("https://192.168.9.15:8581/https", nullptr, nullptr, &res, resHeader);
	if (ret == 0) {
		std::cout << res << std::endl;
	}
}

int main()
{
	http::CCurlHttpClient::init();
	baseRequest();
	// uploadTest();
	// downloadTest();
	// redirTest();
	// urlParamTest();
	// http::CCurlHttpClient::init(500, "ca.pem", "outgoing.CertwithKey.pem", "IoM@1234");
	// httpsTest();
	// multipartTest();
	//http::CCurlHttpClient::init(500, "ca-cert.pem", "client-cert-key.pem", "test");
	// customHttpsTest();

	std::system("pause");

	return 0;
}
