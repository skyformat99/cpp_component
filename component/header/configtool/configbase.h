#ifndef __CONFIG_BASE_CBE5BEADAE0F4A418983B2B3F5901687_H__
#define __CONFIG_BASE_CBE5BEADAE0F4A418983B2B3F5901687_H__

#include <string>

namespace configtool
{

class CConfigBase
{
public:
	explicit CConfigBase(const std::string &);
	virtual ~CConfigBase();

public:
	const std::string &content();

protected:
	void load();

protected:
	virtual void defaultConfig(std::string &) = 0;

private:
	std::string m_path;
	std::string m_content;
};

}

#endif // __CONFIG_BASE_CBE5BEADAE0F4A418983B2B3F5901687_H__
