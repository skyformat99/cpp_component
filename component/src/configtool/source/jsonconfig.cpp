#include "configtool/jsonconfig.h"
#include <memory>

namespace configtool
{

static Json::CharReaderBuilder gBuilder;

CJsonConfig::CJsonConfig(const std::string &path)
	: CConfigBase(path)
{
}

CJsonConfig::~CJsonConfig()
{
}

void CJsonConfig::setJsonValue(const Json::Value &value)
{
	m_value = value;
	load();
}

bool CJsonConfig::getJsonValue(Json::Value &value)
{
	return fromString(content(), value);
}

void CJsonConfig::defaultConfig(std::string &content)
{
	content.assign(m_value.toStyledString());
}

bool CJsonConfig::fromString(const std::string &json, Json::Value &value)
{
	JSONCPP_STRING errs;
	bool result = false;
	std::unique_ptr<Json::CharReader> reader(gBuilder.newCharReader());
	result = reader->parse(json.c_str(), json.c_str() + json.size(), &value, &errs);
	return result;
}

}
