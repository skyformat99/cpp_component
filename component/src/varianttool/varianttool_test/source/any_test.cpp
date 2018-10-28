#include <iostream>
#include "any_test.h"

namespace varianttool
{

CAnyTest::CAnyTest()
{
}

CAnyTest::~CAnyTest()
{
}

void CAnyTest::test()
{
	CAny any = CAnyTest::CTest();
	std::cout << any.cast<CAnyTest::CTest>().call() << std::endl;
}

}
