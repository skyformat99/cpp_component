#ifndef __CHARSET_CONV_47938CDB5E454C61A3E4F8AD08D6F38B_H__
#define __CHARSET_CONV_47938CDB5E454C61A3E4F8AD08D6F38B_H__

#include <string>
#include <memory>

namespace charset
{

class CCharsetConv
{
public:
	explicit CCharsetConv();
	virtual ~CCharsetConv();

public:
	void utf8Togbk(const std::string &utf8, std::string &gbk);
	void gbkToutf8(const std::string &gbk, std::string &utf8);
	void unicodeToutf8(const std::string &unicode, std::string &utf8);
	void utf8Tounicode(const std::string &utf8, std::string &unicode);
	std::string ws2s(const std::wstring &ws);
	std::wstring s2ws(const std::string& s);
	std::unique_ptr<wchar_t> toWchar(const char *src);
	std::unique_ptr<char> toChar(const wchar_t *src);

private:
#ifdef WIN32
	std::string wstring2utf8string(const std::wstring &str);
	std::wstring utf8string2wstring(const std::string &str);
	std::string wstring2string(const std::wstring &str, const std::string &locale);
	std::wstring string2wstring(const std::string &str, const std::string &locale);
#endif
	void unicode2wstring(const std::string &unicode, std::wstring &ws);
	void wstring2utf8(const std::wstring &ws, std::string &s);
	void c2w(const std::string &s, std::wstring &ws);
};

}

#endif