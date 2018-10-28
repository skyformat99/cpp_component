#ifndef __STRING_TOOL_H__
#define __STRING_TOOL_H__

#include <string>
#include <vector>

namespace stringtool
{

class CStringTools
{
public:
	explicit CStringTools();
	virtual ~CStringTools();

public:
	std::vector<std::string> split(const std::string &s, const std::string &pattern);
	void split(const std::string &s, const std::string &pattern, std::vector<std::string> &subs);
	void trim(std::string &s);
	std::string trim2(const std::string &s);
	void trims(std::string &s);
	std::string replaceAll(const std::string &src, const std::string &srcSub, const std::string &dstSub);
};

class CRand
{
public:
	explicit CRand();
	virtual ~CRand();

public:
	void genTimeRand(std::string &rd);
};

}

#endif
