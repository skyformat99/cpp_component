#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <ctime>
#include "stringtool.h"

namespace stringtool
{

CStringTools::CStringTools()
{
}

CStringTools::~CStringTools()
{
}

std::vector<std::string> CStringTools::split(const std::string &s, const std::string &pattern)
{
	std::string str(s);
	std::string::size_type pos;
	std::vector<std::string> result;
	str += pattern;
	size_t size = str.size();

	for (size_t i = 0; i < size; i++)
	{
		pos = str.find(pattern, i);
		if (pos < size)
		{
			std::string s = str.substr(i, pos - i);
			result.push_back(s);
			i = pos + pattern.size() - 1;
		}
	}
	return result;
}

void CStringTools::split(const std::string &s, const std::string &pattern, std::vector<std::string> &subs)
{
	std::string str(s);
	subs.clear();
	std::string::size_type pos;
	str += pattern;
	size_t size = str.size();

	for (size_t i = 0; i < size; i++)
	{
		pos = str.find(pattern, i);
		if (pos < size)
		{
			std::string s = str.substr(i, pos - i);
			subs.push_back(s);
			i = pos + pattern.size() - 1;
		}
	}
}

std::string CStringTools::trim2(const std::string &s)
{
	std::string tmp = s;
	trim(tmp);
	return tmp;
}

void CStringTools::trim(std::string &s)
{
	if (!s.empty())
	{
		s.erase(0, s.find_first_not_of(" "));
		s.erase(s.find_last_not_of(" ") + 1);
	}
}

void CStringTools::trims(std::string &s)
{
	int index = 0;
	if (!s.empty())
	{
		while ((index = s.find(' ', index)) != std::string::npos)
		{
			s.erase(index, 1);
		}
	}
}

std::string CStringTools::replaceAll(const std::string &src, const std::string &srcSub, const std::string &dstSub)
{
	std::string s(src);
	size_t index = 0;
	while (1)
	{
		index = s.find(srcSub);
		if (index == -1) break;
		s.replace(index, srcSub.size(), "");
		s.insert(index, dstSub);
	}
	return s;
}


/////////////////////////////////////
CRand::CRand()
{
}

CRand::~CRand()
{
}

void CRand::genTimeRand(std::string &rd)
{
	srand(time(nullptr));
	rd = std::to_string(rand());
}

}