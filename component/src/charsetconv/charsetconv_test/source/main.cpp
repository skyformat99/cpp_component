#include "charsetconv/charsetconv.h"
#include <iostream>
#include <thread>
#include <string>
#include <regex>
#include <fstream>
#include <uchar.h>

void utf8TogbkTest()
{
	charset::CCharsetConv charsetConv;
	std::string utf8(u8"你好");
	std::string gbk("");
	charsetConv.utf8Togbk(utf8, gbk);
	std::cout << gbk << std::endl;

	std::ofstream of("gbk_out.txt");
	of << utf8;
	of.close();
}

void gbkToutf8()
{
	charset::CCharsetConv charsetConv;
	std::string gbk("你好");
	std::string utf8("");
	charsetConv.gbkToutf8(gbk, utf8);
	std::cout << utf8 << std::endl;

	std::ofstream of("utf_out.txt");
	of << utf8;
	of.close();
}

size_t UTF16StrLen(const char16_t *utf16String)
{
	if (utf16String == NULL)
		return 0;

	size_t index;
	for (index = 0; utf16String[index] != u'\0'; index++);
	return index;
}

size_t UTF16ToUTF8(char *mbBuffer, const char16_t *utf16String)
{
	if (mbBuffer == NULL || utf16String == NULL)
		return 0;

	mbstate_t state = {};

	size_t mbIndex = 0;
	for (int utf16Index = 0; utf16String[utf16Index] != u'\0'; utf16Index++)
	{
		const size_t length = c16rtomb(&mbBuffer[mbIndex], utf16String[utf16Index], &state);
		mbIndex += length;
	}

	mbBuffer[mbIndex] = '\0';

	return mbIndex;
}

void unicodeToutf8()
{
#if 0
	const char16_t *tmp = u"\u5df2\u8bbe\u9632";
	char *buf = new char[UTF16StrLen(tmp)];
	UTF16ToUTF8(buf, tmp);

	if (std::string(buf) == u8"已设防") {
		std::cout << "success" << std::endl;
	}

	charset::CCharsetConv charsetConv;
	std::string gbk("");
	charsetConv.utf8Togbk(buf, gbk);
	std::cout << gbk << std::endl;
#endif

	std::string u16le_byte_str = "\\u4f60\\u597d";
	charset::CCharsetConv charsetConv;
	std::string utf8;
	charsetConv.unicodeToutf8(u16le_byte_str, utf8);
	std::cout << "utf8: " << utf8 << std::endl;

	std::string gbk("");
	charsetConv.utf8Togbk(utf8, gbk);
	std::cout << "gbk: " << gbk << std::endl;
}

int main()
{
	std::cout << "----------utf8 to gbk-----------" << std::endl;
	utf8TogbkTest();
	std::cout << "----------utf8 to gbk-----------" << std::endl;

	std::cout << "----------gbk to utf8-----------" << std::endl;
	gbkToutf8();
	std::cout << "----------gbk to utf8-----------" << std::endl;

	std::cout << "----------unique to utf8-----------" << std::endl;
	unicodeToutf8();
	std::cout << "----------unique to utf8-----------" << std::endl;

	std::system("pause");

	return 0;
}
