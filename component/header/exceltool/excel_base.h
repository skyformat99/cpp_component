#ifndef __EXCEL_BASE_F7C2890521CF4F09B2268C1EA0B54E2B_H__
#define __EXCEL_BASE_F7C2890521CF4F09B2268C1EA0B54E2B_H__

#include <string>
#include <vector>
#include <functional>

namespace ospathtool
{
class COsPath;
class COs;
}

namespace stringtool
{
class CStringTools;
}

namespace exceltool
{

class CExcelBase
{
public:
	typedef std::vector<std::string> row;
	typedef std::vector<std::vector<std::string>> mat;

public:
	explicit CExcelBase(const std::string &path);
	virtual ~CExcelBase();

public:
	bool write(const mat &data, std::function<void(const row &r)> fn = nullptr);
	bool read(mat &data, std::function<void(const row &r)> fn = nullptr);

protected:
	virtual std::string fileExt() = 0;
	virtual std::string splitChar() = 0;

private:
	std::string m_path;
	ospathtool::COsPath *m_ospath;
	stringtool::CStringTools *m_stringTool;
};

}

#endif // __EXCEL_BASE_F7C2890521CF4F09B2268C1EA0B54E2B_H__
