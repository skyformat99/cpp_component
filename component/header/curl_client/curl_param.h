#ifndef __CURL_PARAM_D01B3B6AB812440CAB5119BEC088B33A_H__
#define __CURL_PARAM_D01B3B6AB812440CAB5119BEC088B33A_H__

#include <string>

namespace http
{

namespace formdata
{
enum EFormDataType
{
	EFORMDATA_TYPE_TEXT = 1,
	EFORMDATA_TYPE_FILE = 2
};

struct TFormData
{
	EFormDataType type;
	std::string formdataName;
	std::string filenameORvalue;

	TFormData()
		: type(EFORMDATA_TYPE_TEXT)
		, formdataName("")
		, filenameORvalue("")
	{}
	TFormData(const EFormDataType &t, const std::string &formName, const std::string &value)
		: type(t)
		, formdataName(formName)
		, filenameORvalue(value)
	{}
};
}

}

#endif // __CURL_PARAM_D01B3B6AB812440CAB5119BEC088B33A_H__
