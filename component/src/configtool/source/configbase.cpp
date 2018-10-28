#include "configtool/configbase.h"
#include <fstream>
#include <sstream>

namespace configtool
{

CConfigBase::CConfigBase(const std::string &path)
	: m_path(path)
	, m_content("")
{
}

CConfigBase::~CConfigBase()
{
}

const std::string &CConfigBase::content()
{
	return m_content;
}

void CConfigBase::load()
{
	std::ifstream readFile(m_path);
	if (readFile) {
		std::stringstream stream;
		stream << readFile.rdbuf();
		m_content = stream.str();
		readFile.close();
	}
	else {
		defaultConfig(m_content);
		std::ofstream file(m_path);
		file << m_content;
		file.flush();
		file.close();
	}
}

}
