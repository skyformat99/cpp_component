#include "tools.h"

#ifdef _MSC_VER
#include <Guiddef.h>
#include <Windows.h>
#endif

#ifdef __GNUC__
#include "uuid/uuid.h"
#endif

namespace websocket
{

CTools::CTools()
{
}

CTools::~CTools()
{
}

std::string CTools::generateUUID()
{
	char buffer[64] = { 0 };
#ifdef _MSC_VER
	GUID guid;
	CoCreateGuid(&guid);
	_snprintf(buffer, sizeof(buffer),
		"%08X%04X%04x%02X%02X%02X%02X%02X%02X%02X%02X",
		guid.Data1, guid.Data2, guid.Data3,
		guid.Data4[0], guid.Data4[1], guid.Data4[2],
		guid.Data4[3], guid.Data4[4], guid.Data4[5],
		guid.Data4[6], guid.Data4[7]);
#elif defined __GNUC__
	uuid_t uu;
	uuid_generate(uu);

	snprintf(buffer, sizeof(buffer),
		"%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X",
		uu[0], uu[1], uu[2], uu[3]
		, uu[4], uu[5], uu[6], uu[7]
		, uu[8], uu[9], uu[10], uu[11]
		, uu[12], uu[13], uu[14], uu[15]);
#endif
	return buffer;
}

}
