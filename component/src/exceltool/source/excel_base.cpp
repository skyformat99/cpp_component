#include "ospathtool/ospathtool.h"
#include "stringtool/stringtool.h"
#include "exceltool/excel_base.h"
#include <iostream>
#include <fstream>

namespace exceltool
{

CExcelBase::CExcelBase(const std::string &path)
	: m_path(path)
	, m_ospath(new ospathtool::COsPath())
	, m_stringTool(new stringtool::CStringTools())
{
}

CExcelBase::~CExcelBase()
{
	if (m_stringTool != nullptr) {
		delete m_stringTool;
		m_stringTool = nullptr;
	}
	if (m_ospath != nullptr) {
		delete m_ospath;
		m_ospath = nullptr;
	}
}

bool CExcelBase::write(const mat &data, std::function<void(const row & r)> fn /* = nullptr */)
{
	std::string dev("");
	std::string mid("");
	std::string filename("");
	std::string fileext("");
	m_ospath->splitepath(m_path, dev, mid, filename, fileext);
	std::string ext(".");
	ext.append(fileExt());
	if (fileext != ext) {
		return false;
	}
	std::ofstream file;
	file.open(m_path, std::ios::out | std::ios::trunc);
	if (!file.is_open()) {
		return false;
	}
	for (auto iter = data.begin(); iter != data.end(); ++iter) {
		size_t i = 0;
		size_t size = iter->size();
		for (auto it = iter->begin(); it != iter->end(); ++it, ++i) {
			file << *it;
			if (i < size - 1) {
				file << splitChar();
			}
			if (fn != nullptr) {
				fn(*iter);
			}
		}
		file << std::endl;
	}
	file.close();
	return true;
}

bool CExcelBase::read(mat &data, std::function<void(const row & r)> fn /* = nullptr */)
{
	std::string dev("");
	std::string mid("");
	std::string filename("");
	std::string fileext("");
	m_ospath->splitepath(m_path, dev, mid, filename, fileext);
	std::string ext(".");
	ext.append(fileExt());
	if (fileext != ext) {
		return false;
	}
	std::ifstream file(m_path);
	if (!file) {
		return false;
	}
	std::string buf("");
	data.clear();
	while (std::getline(file, buf)) {
		std::vector<std::string> strs;
		m_stringTool->split(buf, splitChar(), strs);
		if (fn != nullptr) {
			fn(strs);
		}
		data.push_back(strs);
	}
	file.close();
	return true;
}

}
