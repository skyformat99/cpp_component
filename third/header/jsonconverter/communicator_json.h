#ifndef __COMMUNICATOR_JSON_H__
#define __COMMUNICATOR_JSON_H__
#include "jsonconverter.h"

namespace communicator {

class CNetInfoJson;
class CConfigJson;
class CMessageBusJson;
class CStatusJson;

class CNetInfoJson : public CJsonConverter
{
public:
	explicit CNetInfoJson() : CJsonConverter() {}
	explicit CNetInfoJson(const char* json, unsigned jsonLen) : CJsonConverter(json, jsonLen) {}
	explicit CNetInfoJson(const Json::Value& root) : CJsonConverter(root) {}
	~CNetInfoJson() {}
	std::string jsonStyle();
	std::string ip(std::string ip = std::string()) const;
	void setip(const std::string& ip);
	int port(int port = 0) const;
	void setport(int port);
	std::string username(std::string username = std::string()) const;
	void setusername(const std::string& username);
	std::string password(std::string password = std::string()) const;
	void setpassword(const std::string& password);
};

class CConfigJson : public CJsonConverter
{
public:
	explicit CConfigJson() : CJsonConverter() {}
	explicit CConfigJson(const char* json, unsigned jsonLen) : CJsonConverter(json, jsonLen) {}
	explicit CConfigJson(const Json::Value& root) : CJsonConverter(root) {}
	~CConfigJson() {}
	std::string jsonStyle();
	std::string system(std::string system = std::string()) const;
	void setsystem(const std::string& system);
	CNetInfoJson net_info(CNetInfoJson net_info = CNetInfoJson()) const;
	void setnet_info(const CNetInfoJson& net_info);
};

class CMessageBusJson : public CJsonConverter
{
public:
	explicit CMessageBusJson() : CJsonConverter() {}
	explicit CMessageBusJson(const char* json, unsigned jsonLen) : CJsonConverter(json, jsonLen) {}
	explicit CMessageBusJson(const Json::Value& root) : CJsonConverter(root) {}
	~CMessageBusJson() {}
	std::string jsonStyle();
	CNetInfoJson message_bus(CNetInfoJson message_bus = CNetInfoJson()) const;
	void setmessage_bus(const CNetInfoJson& message_bus);
};

class CStatusJson : public CJsonConverter
{
public:
	explicit CStatusJson() : CJsonConverter() {}
	explicit CStatusJson(const char* json, unsigned jsonLen) : CJsonConverter(json, jsonLen) {}
	explicit CStatusJson(const Json::Value& root) : CJsonConverter(root) {}
	~CStatusJson() {}
	std::string jsonStyle();
	std::vector<std::string> topics() const;
	void settopics(const std::vector<std::string>& topicsVector);
};

}
#endif //__COMMUNICATOR_JSON_H__
