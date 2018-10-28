#include "configtool/kvjsonconfig.h"
#include <memory>

namespace configtool
{

static Json::CharReaderBuilder gBuilder;

CKvJsonConfig::CKvJsonConfig(const std::string &path)
	: CConfigBase(path)
	, m_root()
{
}

CKvJsonConfig::~CKvJsonConfig()
{
}

void CKvJsonConfig::load()
{
	CConfigBase::load();
}

void CKvJsonConfig::defaultConfig(std::string &str)
{
	str = m_root.toStyledString();
}

void CKvJsonConfig::_string_(const std::string &key, const Json::Value &value, std::string &data)
{
	data = value[key].asString();
}

void CKvJsonConfig::_int_(const std::string &key, const Json::Value &value, int &data)
{
	data = value[key].asInt();
}

void CKvJsonConfig::_int64_(const std::string &key, const Json::Value &value, int64_t &data)
{
	data = value[key].asInt64();
}

void CKvJsonConfig::_uint_(const std::string &key, const Json::Value &value, uint32_t &data)
{
	data = value[key].asUInt();
}

void CKvJsonConfig::_uint64_(const std::string &key, const Json::Value &value, uint64_t &data)
{
	data = value[key].asUInt64();
}

void CKvJsonConfig::_bool_(const std::string &key, const Json::Value &value, bool &data)
{
	data = value[key].asBool();
}

void CKvJsonConfig::_double_(const std::string &key, const Json::Value &value, double &data)
{
	data = value[key].asDouble();
}

void CKvJsonConfig::_float_(const std::string &key, const Json::Value &value, float &data)
{
	data = value[key].asFloat();
}

bool CKvJsonConfig::fromString(const std::string &json, Json::Value &value)
{
	JSONCPP_STRING errs;
	bool result = false;
	std::unique_ptr<Json::CharReader> reader(gBuilder.newCharReader());
	result = reader->parse(json.c_str(), json.c_str() + json.size(), &value, &errs);
	return result;
}

void CKvJsonConfig::firstInt(const std::string &key, int &data)
{
	reload();
	_int_(key, m_root, data);
}

void CKvJsonConfig::firstInt64(const std::string &key, int64_t &data)
{
	reload();
	_int64_(key, m_root, data);
}

void CKvJsonConfig::firstUInt(const std::string &key, uint32_t &data)
{
	reload();
	_uint_(key, m_root, data);
}

void CKvJsonConfig::firstUInt64(const std::string &key, uint64_t &data)
{
	reload();
	_uint64_(key, m_root, data);
}

void CKvJsonConfig::firstString(const std::string &key, std::string &data)
{
	reload();
	_string_(key, m_root, data);
}

void CKvJsonConfig::firstBool(const std::string &key, bool &data)
{
	reload();
	_bool_(key, m_root, data);
}

void CKvJsonConfig::firstDouble(const std::string &key, double &data)
{
	reload();
	_double_(key, m_root, data);
}

void CKvJsonConfig::firstFloat(const std::string &key, float &data)
{
	reload();
	_float_(key, m_root, data);
}

void CKvJsonConfig::secondInt(const std::string &k1, const std::string &k2, int &data)
{
	reload();
	_int_(k2, m_root[k1], data);
}

void CKvJsonConfig::secondInt64(const std::string &k1, const std::string &k2, int64_t &data)
{
	reload();
	_int64_(k2, m_root[k1], data);
}

void CKvJsonConfig::secondUInt(const std::string &k1, const std::string &k2, uint32_t &data)
{
	reload();
	_uint_(k2, m_root[k1], data);
}

void CKvJsonConfig::secondUInt64(const std::string &k1, const std::string &k2, uint64_t &data)
{
	reload();
	_uint64_(k2, m_root[k1], data);
}

void CKvJsonConfig::secondString(const std::string &k1, const std::string &k2, std::string &data)
{
	reload();
	_string_(k2, m_root[k1], data);
}

void CKvJsonConfig::secondBool(const std::string &k1, const std::string &k2, bool &data)
{
	reload();
	_bool_(k2, m_root[k1], data);
}

void CKvJsonConfig::secondDouble(const std::string &k1, const std::string &k2, double &data)
{
	reload();
	_double_(k2, m_root[k1], data);
}

void CKvJsonConfig::secondFloat(const std::string &k1, const std::string &k2, float &data)
{
	reload();
	_float_(k2, m_root[k1], data);
}

void CKvJsonConfig::reload()
{
	load();
	fromString(content(), m_root);
}

}
