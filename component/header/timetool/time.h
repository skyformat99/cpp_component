#ifndef __TIME_6CA396F2AC234205A649E71EC8C05C5D_H__
#define __TIME_6CA396F2AC234205A649E71EC8C05C5D_H__

#include <stdint.h>

namespace timetool
{

class CTime
{
public:
	explicit CTime();
	virtual ~CTime();

public:
	int64_t nowMS() const;
	int64_t nowNS() const;
};

}

#endif // __TIME_6CA396F2AC234205A649E71EC8C05C5D_H__
