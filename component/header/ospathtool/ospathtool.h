#ifndef __OS_PATH_TOOL_H__
#define __OS_PATH_TOOL_H__

#include <string>

namespace ospathtool
{

class COs
{
public:
	explicit COs();
	virtual ~COs();

public:
	bool remove(const std::string &path);
	bool rmdir(const std::string &path);
	bool mkdir(const std::string &path, int mode = 0);
	bool mkdirs(const std::string &path, int mode = 0);
};

class COsPath
{
public:
	explicit COsPath();
	virtual ~COsPath();

public:
	bool exists(const std::string &path);
	std::string relpath(const std::string &path);
	std::string abspath(const std::string &path);
	std::string curdir();
	bool isdir(const std::string &path);
	bool isfile(const std::string &path);
	std::string dirname(const std::string &path);
	bool dirname(const std::string &path, std::string &dir);
	void splitepath(const std::string &path, std::string &dev, std::string &mid, std::string &filename, std::string &fileext);
};

class CShutil
{
public:
	explicit CShutil();
	virtual ~CShutil();

public:
	bool copy(const std::string &src, const std::string &dst);
};

}

#endif
