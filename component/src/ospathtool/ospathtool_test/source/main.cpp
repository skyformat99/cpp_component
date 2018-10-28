#include "ospathtool/ospathtool.h"
#include <iostream>
#include <thread>

void mkdirs_test()
{
	ospathtool::COs os;
	os.mkdirs("test1/test2/test3");
	os.mkdirs("test1/test2/test3/test4/test5");
	os.mkdirs("test2/test2/test3");
	os.mkdirs("test2/test2/test3/test4/test5");
	os.mkdirs("test3/test2/test3/test4/test5");
}

void path_splite_test()
{
	ospathtool::COsPath ospath;
	std::string dev;
	std::string mid;
	std::string filename;
	std::string fileext;
	ospath.splitepath("1/2/3/xxx.jpg", dev, mid, filename, fileext);
	std::cout << dev << "," << mid << ", " << filename << ", " << fileext << std::endl;
}

int main()
{
	path_splite_test();

	std::system("pause");

	return 0;
}
