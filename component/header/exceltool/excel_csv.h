#ifndef __EXCEL_CSV_FB3BBF7E2ACF4063B2E54764D02A13DA_H__
#define __EXCEL_CSV_FB3BBF7E2ACF4063B2E54764D02A13DA_H__

#include "excel_base.h"

namespace exceltool
{

class CExcelCsv : public CExcelBase
{
public:
	explicit CExcelCsv(const std::string &path);
	virtual ~CExcelCsv();

private:
	virtual std::string fileExt();
	virtual std::string splitChar();
};

}

#endif // __EXCEL_CSV_FB3BBF7E2ACF4063B2E54764D02A13DA_H__
