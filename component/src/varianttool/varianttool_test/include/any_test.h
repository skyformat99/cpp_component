#ifndef __ANY_TEST_H__
#define __ANY_TEST_H__

#include "varianttool/any.h"

namespace varianttool
{

class CAnyTest
{
public:
	explicit CAnyTest();
	virtual ~CAnyTest();

public:
	void test();

private:
	class CTest
	{
	public:
		explicit CTest() {}
		virtual ~CTest() {}

	public:
		const char *call()
		{
			return "call ... ";
		}
	};
};

}

#endif // __ANY_TEST_H__
