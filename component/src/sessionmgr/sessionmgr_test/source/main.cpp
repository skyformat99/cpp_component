#include <string>
#include <iostream>
#include "sessionmgr/sessionmgr.h"

int main()
{
	session::CSessionMgr sessionMgr;
	std::string sessionId;
	sessionMgr.createSession(60 * 1000, sessionId);
	sessionMgr.createSession(10 * 1000, sessionId);

	std::system("pause");

	return 0;
}
