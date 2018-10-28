#ifndef __EVENT_STORE_JSON_H__
#define __EVENT_STORE_JSON_H__
#include "jsonconverter.h"

namespace event_store {

class CResultJson;
class CPutEventJson;
class CEventStatusJson;
class CHeadResourceJson;

class CResultJson : public CJsonConverter
{
public:
	explicit CResultJson() : CJsonConverter() {}
	explicit CResultJson(const char* json, unsigned jsonLen) : CJsonConverter(json, jsonLen) {}
	explicit CResultJson(const Json::Value& root) : CJsonConverter(root) {}
	~CResultJson() {}
	std::string jsonStyle();
	int result(int result = 0) const;
	void setresult(int result);
	std::string result_string(std::string result_string = std::string()) const;
	void setresult_string(const std::string& result_string);
};

class CPutEventJson : public CJsonConverter
{
public:
	explicit CPutEventJson() : CJsonConverter() {}
	explicit CPutEventJson(const char* json, unsigned jsonLen) : CJsonConverter(json, jsonLen) {}
	explicit CPutEventJson(const Json::Value& root) : CJsonConverter(root) {}
	~CPutEventJson() {}
	std::string jsonStyle();
	std::string action(std::string action = std::string()) const;
	void setaction(const std::string& action);
	std::string uri(std::string uri = std::string()) const;
	void seturi(const std::string& uri);
	int revision(int revision = 0) const;
	void setrevision(int revision);
	std::string type(std::string type = std::string()) const;
	void settype(const std::string& type);
};

class CEventStatusJson : public CJsonConverter
{
public:
	explicit CEventStatusJson() : CJsonConverter() {}
	explicit CEventStatusJson(const char* json, unsigned jsonLen) : CJsonConverter(json, jsonLen) {}
	explicit CEventStatusJson(const Json::Value& root) : CJsonConverter(root) {}
	~CEventStatusJson() {}
	std::string jsonStyle();
	int revision(int revision = 0) const;
	void setrevision(int revision);
	std::string type(std::string type = std::string()) const;
	void settype(const std::string& type);
};

class CHeadResourceJson : public CJsonConverter
{
public:
	explicit CHeadResourceJson() : CJsonConverter() {}
	explicit CHeadResourceJson(const char* json, unsigned jsonLen) : CJsonConverter(json, jsonLen) {}
	explicit CHeadResourceJson(const Json::Value& root) : CJsonConverter(root) {}
	~CHeadResourceJson() {}
	std::string jsonStyle();
	bool exist(bool exist = false) const;
	void setexist(bool exist);
	int revision(int revision = 0) const;
	void setrevision(int revision);
};

}
#endif //__EVENT_STORE_JSON_H__
