#ifndef __TOOLS_0249D01B19EE43BF8F4D4F3A8E83BA7C_H__
#define __TOOLS_0249D01B19EE43BF8F4D4F3A8E83BA7C_H__

#include <string>

namespace websocket
{

class CTools
{
public:
	explicit CTools();
	virtual ~CTools();

public:
	std::string generateUUID();
};

}

#endif // __TOOLS_0249D01B19EE43BF8F4D4F3A8E83BA7C_H__
