#ifndef __UUID_9EAF3D3AC5E241FA947BE08690118FCE_H__
#define __UUID_9EAF3D3AC5E241FA947BE08690118FCE_H__

#include <string>

namespace randtool
{

class CUuid
{
public:
	explicit CUuid();
	virtual ~CUuid();

public:
	std::string generator();
};

}

#endif // __UUID_9EAF3D3AC5E241FA947BE08690118FCE_H__
