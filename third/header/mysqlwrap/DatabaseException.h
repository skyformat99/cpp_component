#pragma once

#include <iostream>
#include <mysql.h>
#include <string>

namespace MySQLWrap {
	class DatabaseException {
	public:
		DatabaseException(const std::string &initialMessage, const int _errno, const char *sqlState, const char *errorMessage);
		DatabaseException(MYSQL *db, const std::string &initialMessage);
		DatabaseException(MYSQL_STMT *stmt, const std::string &initialMessage);

		friend std::ostream &operator<<(std::ostream &out, const DatabaseException &exp);

	private:
		int m_errno;
		std::string _sqlState;
		std::string _initialMessage;
		std::string _errorMessage;
	};
}
