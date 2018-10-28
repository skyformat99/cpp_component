#ifndef __JSON_CONVERTER_H__
#define __JSON_CONVERTER_H__
#include "json/json.h"
#include <mutex>
#include <memory>

static Json::CharReaderBuilder gBuilder;

class CJsonConverter
{
public:
	explicit CJsonConverter() : m_root() {}
	explicit CJsonConverter(const char* json, unsigned jsonLen) : m_root() { fromString(std::string(json, jsonLen)); }
	explicit CJsonConverter(const Json::Value& root) : m_root(root) {}
	virtual ~CJsonConverter() {}
	virtual std::string jsonStyle() { return ""; }
	bool fromString(const std::string& json)
	{
		JSONCPP_STRING errs;
		bool result = false;
		std::unique_ptr<Json::CharReader> reader(gBuilder.newCharReader());
		result = reader->parse(json.c_str(), json.c_str() + json.size(), &m_root, &errs);
		return result;
	}
	std::string toString() { return m_root.toStyledString(); }
	const Json::Value& root() const { return m_root; }
	void setRoot(const Json::Value& root) { m_root = root; }
protected:
	Json::Value m_root;	
};

#endif //__JSON_CONVERTER_H__
