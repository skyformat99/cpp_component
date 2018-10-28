#ifndef __CONFIG_TOOL_29E6818D26784ACCA9438F1BFD501BE8_H__
#define __CONFIG_TOOL_29E6818D26784ACCA9438F1BFD501BE8_H__

#include "configbase.h"
#include "json/json.h"

namespace configtool
{

class CJsonConfig : public CConfigBase
{
public:
	explicit CJsonConfig(const std::string &);
	virtual ~CJsonConfig();

public:
	void setJsonValue(const Json::Value &value);
	bool getJsonValue(Json::Value &value);

protected:
	virtual void defaultConfig(std::string &);

private:
	bool fromString(const std::string &, Json::Value &value);

private:
	Json::Value m_value;
};

}

#endif
