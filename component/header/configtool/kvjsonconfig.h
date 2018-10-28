#ifndef __KVJSON_CONFIG_2EBCFBB952C540C29645833843693089_H__
#define __KVJSON_CONFIG_2EBCFBB952C540C29645833843693089_H__

#include "configbase.h"
#include "json/json.h"
#include <map>
#include <vector>
#include <stdint.h>

namespace configtool
{

class CKvJsonConfig : public CConfigBase
{
public:
	template<typename T>
	struct KV
	{
		std::string key;
		T value;

		KV(const std::string &k, const T &v)
			: key(k)
			, value(v)
		{}
	};

public:
	explicit CKvJsonConfig(const std::string &);
	virtual ~CKvJsonConfig();

public:
	template<typename T>
	void encoding(const KV<T> &data)
	{
		m_root[data.key] = data.value;
	}
	template<typename T>
	void encoding(const std::string &key, const KV<T> &value)
	{
		Json::Value v;
		v[value.key] = value.value;
		m_root[key] = v;
	}
	void load();
	void firstInt(const std::string &key, int &data);
	void firstInt64(const std::string &key, int64_t &data);
	void firstUInt(const std::string &key, uint32_t &data);
	void firstUInt64(const std::string &key, uint64_t &data);
	void firstString(const std::string &key, std::string &data);
	void firstBool(const std::string &key, bool &data);
	void firstDouble(const std::string &key, double &data);
	void firstFloat(const std::string &key, float &data);
	void secondInt(const std::string &k1, const std::string &k2, int &data);
	void secondInt64(const std::string &k1, const std::string &k2, int64_t &data);
	void secondUInt(const std::string &k1, const std::string &k2, uint32_t &data);
	void secondUInt64(const std::string &k1, const std::string &k2, uint64_t &data);
	void secondString(const std::string &k1, const std::string &k2, std::string &data);
	void secondBool(const std::string &k1, const std::string &k2, bool &data);
	void secondDouble(const std::string &k1, const std::string &k2, double &data);
	void secondFloat(const std::string &k1, const std::string &k2, float &data);

private:
	virtual void defaultConfig(std::string &str);
	void _string_(const std::string &key, const Json::Value &value, std::string &data);
	void _int_(const std::string &key, const Json::Value &value, int &data);
	void _int64_(const std::string &key, const Json::Value &value, int64_t &data);
	void _uint_(const std::string &key, const Json::Value &value, uint32_t &data);
	void _uint64_(const std::string &key, const Json::Value &value, uint64_t &data);
	void _bool_(const std::string &key, const Json::Value &value, bool &data);
	void _double_(const std::string &key, const Json::Value &value, double &data);
	void _float_(const std::string &key, const Json::Value &value, float &data);
	bool fromString(const std::string &json, Json::Value &value);
	void reload();

private:
	Json::Value m_root;
};

}

#endif // __KVJSON_CONFIG_2EBCFBB952C540C29645833843693089_H__
