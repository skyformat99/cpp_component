#include "exceltool/excel_csv.h"

namespace exceltool
{

CExcelCsv::CExcelCsv(const std::string &path)
	: CExcelBase(path)
{
}

CExcelCsv::~CExcelCsv()
{
}

std::string CExcelCsv::fileExt()
{
	return "csv";
}

std::string CExcelCsv::splitChar()
{
	return ",";
}

}
