#include "exceltool/excel_csv.h"
#include <iostream>
#include <thread>

void excelWriteTest()
{
	exceltool::CExcelCsv excel("test.csv");
	exceltool::CExcelBase::mat m;
	exceltool::CExcelBase::row r;
	r.push_back("1");
	r.push_back("liujun");
	r.push_back("100");
	m.push_back(r);
	excel.write(m);
}

void excelReadTest()
{
	exceltool::CExcelCsv excel("test.csv");
	exceltool::CExcelBase::mat m;
	excel.read(m, [] (const exceltool::CExcelBase::row &row) {
		for (auto iter = row.begin(); iter != row.end(); ++iter) {
			std::cout << *iter << std::endl;
		}
	});
}

int main()
{
	// excelWriteTest();
	excelReadTest();

#ifdef WIN32
	std::system("pause");
#endif

	return 0;
}

