#include "ospathtool.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifdef WIN32
#include <io.h>
#include <windows.h>
#include <direct.h>
#endif

#ifdef _LINUX_
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <limits.h>
#endif

#include <regex>

namespace ospathtool
{
COs::COs()
{
}

COs::~COs()
{
}

bool COs::mkdir(const std::string &path, int mode)
{
	int ret = 0;
#ifdef WIN32
	ret = ::mkdir(path.c_str());
	if (ret == -1) return false;
#elif defined _LINUX_
	ret = ::mkdir(path.c_str(), mode);
	if (ret == -1) return false;
#endif
	return true;
}

bool COs::mkdirs(const std::string &path, int mode /* = 0 */)
{
	bool ret = true;
	COsPath ospath;
	if (ospath.exists(path) == false)
	{
		std::string dir;
		bool b = ospath.dirname(path, dir);
		if (b == false)
		{
			mkdir(path, mode);
		}
		else
		{
			mkdirs(dir, mode);
			mkdir(path, mode);
		}
	}
	else
	{
		return true;
	}

	return ret;
}

bool COs::remove(const std::string &path)
{
	int ret = ::remove(path.c_str());
	if (ret == -1) return false;
	return true;
}

bool COs::rmdir(const std::string &path)
{
	int ret = ::rmdir(path.c_str());
	if (ret == -1) return false;
	return true;
}

///////////////////////////////////////
COsPath::COsPath()
{
}

COsPath::~COsPath()
{
}

bool COsPath::exists(const std::string &path)
{
	int ret = access(path.c_str(), 0);
	if (ret == -1)
	{
		return false;
	}
	return true;
}

std::string COsPath::relpath(const std::string &path)
{
#ifdef WIN32
	const static int max = 1024;
	char buf[max];
	memset(&buf, 0, max);
	_fullpath(buf, path.c_str(), max);
	return buf;
#elif defined _LINUX_
	const static int max = 1024;
	char buf[max];
	memset(&buf, 0, max);
	realpath(path.c_str(), buf);
	return buf;
#endif
}

std::string COsPath::abspath(const std::string &path)
{
#ifdef WIN32
	const static int max = 4096;
	char buf[max];
	memset(&buf, 0, max);
	_fullpath(buf, path.c_str(), max);
	return buf;
#elif defined _LINUX_
	const static int max = 40960;
	char buf[max];
	memset(&buf, 0, max);
	realpath(path.c_str(), buf);
	return buf;
#endif
}

std::string COsPath::curdir()
{
	const static int max = 4096;
	char buf[max];
	memset(&buf, 0, max);
	getcwd(buf, sizeof(buf));
	return buf;
}

bool COsPath::isfile(const std::string &path)
{
	return !isdir(path);
}

bool COsPath::isdir(const std::string &path)
{
#ifdef WIN32
	struct _stat fileStat;
	if ((_stat(path.c_str(), &fileStat) == 0) && (fileStat.st_mode & _S_IFDIR))
	{
		return true;
	}
	return false;
#elif defined _LINUX_
	struct stat buf;
	if (lstat(path.c_str(), &buf) < 0) {
		return false;
	}
	int ret = __S_IFDIR & buf.st_mode;
	if (ret) {
		return true;
	}
	return true;
#endif
}

bool COsPath::dirname(const std::string &path, std::string &dir)
{
	std::string tmp = path;
	size_t n = tmp.find_last_of("/\\");
	if (n == -1) return false;
	std::string filename = tmp.substr(n + 1);
	dir = tmp.erase(n, filename.size() + 1);
	return true;
}

std::string COsPath::dirname(const std::string &path)
{
	std::string dir;
	std::string tmp = path;
	size_t n = tmp.find_last_of("/\\");
	if (n == -1) return "";
	std::string filename = tmp.substr(n + 1);
	dir = tmp.erase(n, filename.size() + 1);
#if 0
	if (isdir(path))
	{
		// get last name
		size_t n = path.find_last_of("/\\");
		dir = path.substr(0, n);
	}
	else
	{
		// remove last name, get last name
		std::string tmp = path;
		size_t n = tmp.find_last_of("/\\");
		std::string filename = tmp.substr(n + 1);
		dir = tmp.erase(n, filename.size() + 1);
	}
#endif
	return dir;
}

#ifndef WIN32
static void _split_whole_name(const char *whole_name, char *fname, char *ext)
{
	char *p_ext;

	p_ext = rindex((char*)whole_name, '.');
	if (NULL != p_ext)
	{
		strcpy(ext, p_ext);
		snprintf(fname, p_ext - whole_name + 1, "%s", whole_name);
	}
	else
	{
		ext[0] = '\0';
		strcpy(fname, whole_name);
	}
}

void splitpath(const char *path, char *drive, char *dir, char *fname, char *ext)
{
	char *p_whole_name;

	drive[0] = '\0';
	if (NULL == path)
	{
		dir[0] = '\0';
		fname[0] = '\0';
		ext[0] = '\0';
		return;
	}

	if ('/' == path[strlen(path)])
	{
		strcpy(dir, path);
		fname[0] = '\0';
		ext[0] = '\0';
		return;
	}

	p_whole_name = rindex((char*)path, '/');
	if (NULL != p_whole_name)
	{
		p_whole_name++;
		_split_whole_name(p_whole_name, fname, ext);

		snprintf(dir, p_whole_name - path, "%s", path);
	}
	else
	{
		_split_whole_name(path, fname, ext);
		dir[0] = '\0';
	}
}
#endif

void COsPath::splitepath(const std::string &path, std::string &dev, std::string &mid, std::string &filename, std::string &fileext)
{
	char _dev[32] = { 0 };
	char _mid[1024] = { 0 };
	char _filename[128] = { 0 };
	char _ext[32] = { 0 };
#ifndef WIN32
	splitpath(path.c_str(), _dev, _mid, _filename, _ext);
#endif
#ifdef WIN32
	_splitpath(path.c_str(), _dev, _mid, _filename, _ext);
#endif
	dev = _dev;
	mid = _mid;
	filename = _filename;
	fileext = _ext;
}

///////////////////////////////////////////////
CShutil::CShutil()
{
}

CShutil::~CShutil()
{
}

bool CShutil::copy(const std::string &src, const std::string &dst)
{
	const static int bufSize = 32 * 1024;
	COsPath ospath;
	COs os;
	if (ospath.exists(src) == false) return false;
	const std::string &dirname = ospath.dirname(dst);
	if (ospath.exists(dirname) == false) {
		os.mkdirs(dirname);
	}
	FILE *in_file, *out_file;
	char data[bufSize];
	size_t bytes_in, bytes_out;
	long len = 0;
	if ((in_file = fopen(src.c_str(), "rb")) == NULL)
	{
		return false;
	}
	if ((out_file = fopen(dst.c_str(), "wb")) == NULL)
	{
		return false;
	}
	while ((bytes_in = fread(data, 1, bufSize, in_file)) > 0)
	{
		bytes_out = fwrite(data, 1, bytes_in, out_file);
		if (bytes_in != bytes_out)
		{
			return false;
		}
		len += bytes_out;
	}
	fclose(in_file);
	fclose(out_file);
	return true;
}

}