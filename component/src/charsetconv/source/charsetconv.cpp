#include "charsetconv/charsetconv.h" 
#include <locale>
#include <string.h>
#include <stdlib.h>
#ifdef WIN32
#include <windows.h>
#include <codecvt>
#endif

// #define debug

namespace charset 
{

CCharsetConv::CCharsetConv()
{
}

CCharsetConv::~CCharsetConv()
{
}

void CCharsetConv::utf8Togbk(const std::string &utf8, std::string &gbk)
{
#ifdef WIN32
	const std::wstring &tmp = utf8string2wstring(utf8);
	std::string format("");
	format.assign("Chinese_China.936");
	gbk = wstring2string(tmp, format);
#elif defined _LINUX_
	if (nullptr == setlocale(LC_ALL, "zh_CN.utf8")) {
#ifdef debug
		printf("1\n");
#endif
		return;
	}
	int unicodeLen = mbstowcs(nullptr, utf8.c_str(), 0);
	if (unicodeLen <= 0) {
#ifdef debug
		printf("2\n");
#endif
		return;
	}
	wchar_t *unicodeStr = (wchar_t *)calloc(sizeof(wchar_t), unicodeLen + 1);
	mbstowcs(unicodeStr, utf8.c_str(), utf8.size());
	if (nullptr == setlocale(LC_ALL, "zh_CN.gbk")) {
#ifdef debug
		printf("3\n");
#endif
		return;
	}
	int gbkLen = wcstombs(nullptr, unicodeStr, 0);
	if (gbkLen <= 0) {
#ifdef debug
		printf("4\n");
#endif
		return;
	}
	char *gbkbuf = new char[gbkLen];
	memset(gbkbuf, 0, gbkLen);
	wcstombs(gbkbuf, unicodeStr, gbkLen);
	gbk = gbkbuf;
	if (gbkbuf != nullptr) {
		delete[] gbkbuf;
		gbkbuf = nullptr;
	}
	free(unicodeStr);
#endif
}

void CCharsetConv::gbkToutf8(const std::string &gbk, std::string &utf8)
{
#ifdef WIN32
	std::string format("");
	format.assign("Chinese_China.936");
	const std::wstring & tmp = string2wstring(gbk, format);
	utf8 = wstring2utf8string(tmp);
#elif defined _LINUX_
	if (nullptr == setlocale(LC_ALL, "zh_CN.gbk")) {
#ifdef debug
		printf("1\n");
#endif
		return;
	}
	int unicodeLen = mbstowcs(nullptr, gbk.c_str(), 0);
	if (unicodeLen <= 0) {
#ifdef debug
		printf("2\n");
#endif
		return;
	}
	wchar_t *unicodeStr = (wchar_t *)calloc(sizeof(wchar_t), unicodeLen + 1);
	mbstowcs(unicodeStr, gbk.c_str(), gbk.size());
	if (nullptr == setlocale(LC_ALL, "zh_CN.utf8")) {
#ifdef debug
		printf("3\n");
#endif
		return;
	}
	int utfLen = wcstombs(nullptr, unicodeStr, 0);
	if (utfLen <= 0) {
#ifdef debug
		printf("4\n");
#endif
		return;
	}
	char *utfbuf = new char[utfLen];
	memset(utfbuf, 0, utfLen);
#ifdef debug
	printf("utflen: %d\n", utfLen);
#endif
	wcstombs(utfbuf, unicodeStr, utfLen);
	free(unicodeStr);
	utf8 = utfbuf;
	if (utfbuf != nullptr) {
		delete[] utfbuf;
		utfbuf = nullptr;
	}
#endif
}

void CCharsetConv::unicodeToutf8(const std::string &unicode, std::string &utf8)
{
	std::wstring ws;
	unicode2wstring(unicode, ws);
	wstring2utf8(ws, utf8);
}

void CCharsetConv::utf8Tounicode(const std::string &utf8, std::string &unicode)
{
	std::wstring ws;
}

void CCharsetConv::unicode2wstring(const std::string &unicode, std::wstring &ws)
{
	ws.clear();
	bool escape = false;
	size_t len = unicode.size();
	int intHex;
	char tmp[5];
	memset(tmp, 0, 5);
	for (int i = 0; i < len; i++)
	{
		char c = unicode[i];
		switch (c)
		{
		case'\\':
		case'%':
			escape = true;
			break;
		case'u':
		case'U':
			if (escape)
			{
				memcpy(tmp, unicode.c_str() + i + 1, 4);
				sscanf(tmp, "%x", &intHex);
				ws.push_back(intHex);
				i += 4;
				escape = false;
			}
			else {
				ws.push_back(c);
			}
			break;
		default:
			ws.push_back(c);
			break;
		}
	}
}

void CCharsetConv::wstring2utf8(const std::wstring &ws, std::string &s)
{
#ifdef _LINUX_
	char *local = setlocale(LC_ALL, "zh_CN.utf8");
	if (local == nullptr) return;
	int utfLen = wcstombs(nullptr, ws.c_str(), 0);
	if (utfLen <= 0) {
		return;
	}
	s.resize(utfLen);
	wcstombs(&s[0], ws.c_str(), utfLen);
#elif defined WIN32
	std::string format("");
	format.assign("Chinese_China.936");
	s = wstring2string(ws, format);
	gbkToutf8(s, s);
#endif
}

void CCharsetConv::c2w(const std::string &s, std::wstring &ws)
{
	size_t size_of_wc;
	size_t destlen = mbstowcs(0, s.c_str(), 0);
	if (destlen == (size_t)(-1))
	{
		return;
	}
	size_of_wc = destlen + 1;
	wchar_t *pw = new wchar_t[size_of_wc];
	mbstowcs(pw, s.c_str(), size_of_wc);
	ws = pw;
	delete pw;
}

#ifdef WIN32
std::string CCharsetConv::wstring2utf8string(const std::wstring& str)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t> > strCnv;
	return strCnv.to_bytes(str);
}

std::wstring CCharsetConv::utf8string2wstring(const std::string& str)
{
	std::wstring_convert< std::codecvt_utf8<wchar_t> > strCnv;
	return strCnv.from_bytes(str);
}

std::string CCharsetConv::wstring2string(const std::wstring& str, const std::string& locale)
{
	typedef std::codecvt_byname<wchar_t, char, std::mbstate_t> F;
	std::wstring_convert<F> strCnv(new F(locale));

	return strCnv.to_bytes(str);
}

std::wstring CCharsetConv::string2wstring(const std::string& str, const std::string& locale)
{
	typedef std::codecvt_byname<wchar_t, char, std::mbstate_t> F;
	std::wstring_convert<F> strCnv(new F(locale));

	return strCnv.from_bytes(str);
}
#endif

std::string CCharsetConv::ws2s(const std::wstring &ws)
{
	std::string curLocale = setlocale(LC_ALL, NULL);
#ifdef WIN32
	setlocale(LC_ALL, "chs");
#elif defined _LINUX_
	setlocale(LC_ALL, "zh_CN.utf8");
#endif

	const wchar_t* _Source = ws.c_str();
	size_t _Dsize = 2 * ws.size() + 1;
	char *_Dest = new char[_Dsize];
	memset(_Dest, 0, _Dsize);
	wcstombs(_Dest, _Source, _Dsize);
	std::string result = _Dest;
	delete[]_Dest;

	setlocale(LC_ALL, curLocale.c_str());
	return result;
}

std::wstring CCharsetConv::s2ws(const std::string& s)
{
	std::string curLocale = setlocale(LC_ALL, NULL);
#ifdef WIN32
	setlocale(LC_ALL, "chs");
#elif defined _LINUX_
	setlocale(LC_ALL, "zh_CN.utf8");
#endif

	const char* _Source = s.c_str();
	size_t _Dsize = s.size() + 1;
	wchar_t *_Dest = new wchar_t[_Dsize];
	wmemset(_Dest, 0, _Dsize);
	mbstowcs(_Dest, _Source, _Dsize);
	std::wstring result = _Dest;
	delete[]_Dest;

	setlocale(LC_ALL, curLocale.c_str());
	return result;
}

std::unique_ptr<wchar_t> CCharsetConv::toWchar(const char *src)
{
	size_t len = strlen(src) + 1;
	if (len <= 1) return nullptr;
	wchar_t *dest = (wchar_t*)malloc(len);
	size_t size = len * sizeof(char);
	memset(dest, 0, size);
	size_t ret = mbstowcs(dest, src, size);
	return std::unique_ptr<wchar_t>(dest);
}

std::unique_ptr<char> CCharsetConv::toChar(const wchar_t *src)
{
	std::string curLocale = setlocale(LC_ALL, NULL);
#ifdef WIN32
	setlocale(LC_ALL, "chs");
#elif defined _LINUX_
	setlocale(LC_ALL, "zh_CN.utf8");
#endif

	size_t len = wcslen(src);
	if (len <= 1) return nullptr;
	size_t size = len * sizeof(wchar_t) + 1;
	char *dest = (char*)malloc(size);
	memset(dest, 0, size);
	size_t ret = wcstombs(dest, src, size);

	setlocale(LC_ALL, curLocale.c_str());

	return std::unique_ptr<char>(dest);
}

#ifdef WIN32
char* WideCharToMulityByte(wchar_t *str)
{
	int len = WideCharToMultiByte(CP_UTF8, 0, str, -1, 0, 0, 0, 0);
	char* output = new char[len + 2];
	WideCharToMultiByte(CP_UTF8, 0, str, -1, output, len + 1, 0, 0);
	return output;
}

wchar_t* MulityByteToWideChar(char *str)
{
	DWORD dwNum = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
	wchar_t *pwText = new wchar_t[dwNum];
	MultiByteToWideChar(CP_UTF8, 0, str, -1, pwText, dwNum);
	return pwText;
}
#endif

}
