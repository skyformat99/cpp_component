#include "uri_filter.h"

namespace msf {

CUriFilter::CUriFilter()
	: m_filters()
{

}

CUriFilter::~CUriFilter()
{
	clean();
}

std::vector<std::string> CUriFilter::sections(const std::string& uri)
{
	std::vector<std::string> sections;
	size_t pos = 0;
	do
	{
		size_t findPos = uri.find('/', pos);
		if (findPos == -1) {
			sections.push_back(uri.substr(pos));
			break;
		}
		if (findPos != pos) {
			sections.push_back(uri.substr(pos, findPos - pos));
		}
		pos = findPos + 1;
	} while (pos != -1);
	return sections;
}

void CUriFilter::subscribe(const std::string& uri, IHttpHandler* handler)
{
	subscribe(sections(uri), handler);
}

void CUriFilter::unsubscribe(const std::string& uri)
{
	unsubscribe(sections(uri));
}

IHttpHandler* CUriFilter::handler(const std::string& uri)
{
	return handler(sections(uri));
}

void CUriFilter::subscribe(const std::vector<std::string>& sections, IHttpHandler* handler, int32_t depth /* = 0 */)
{
	if (depth < sections.size()) {
		const std::string& section = sections.at(depth);
		auto it = m_filters.find(section);
		if (depth == sections.size() - 1) {
			if (m_filters.end() == it) {
				m_filters.insert(std::make_pair(section, std::make_pair(new CUriFilter, handler)));
			}
			else {
				it->second.second = handler;
			}
		}
		else {
			CUriFilter* filter = nullptr;
			if (m_filters.end() == it) {
				filter = new CUriFilter;
				m_filters.insert(std::make_pair(section, std::make_pair(filter, nullptr)));
			}
			else {
				filter = it->second.first;
			}
			filter->subscribe(sections, handler, depth + 1);
		}
	}
}

void CUriFilter::unsubscribe(const std::vector<std::string>& sections, int32_t depth /* = 0 */)
{
	if (depth < sections.size()) {
		const std::string& section = sections.at(depth);
		auto it = m_filters.find(section);
		if (depth == sections.size() - 1) {
			if (m_filters.end() != it) {
				it->second.second = nullptr;
				if (it->second.first->m_filters.empty()) {
					delete it->second.first;
					m_filters.erase(it);
				}
			}
		}
		else if (m_filters.end() != it) {
			it->second.first->unsubscribe(sections, depth + 1);
			if (it->second.first->m_filters.empty()) {
				delete it->second.first;
				m_filters.erase(it);
			}
		}
	}
}

IHttpHandler* CUriFilter::handler(const std::vector<std::string>& sections, int32_t depth /* = 0 */)
{
	IHttpHandler* handler = nullptr;
	if (depth < sections.size()) {
		const std::string& section = sections.at(depth);
		auto it = m_filters.find(section);
		if (m_filters.end() != it) {
			if (depth == sections.size() - 1) {
				handler = it->second.second;
			}
			else {
				handler = it->second.first->handler(sections, depth + 1);
			}
		}

		if (handler) return handler;

		it = m_filters.find("+");
		if (m_filters.end() != it) {
			if (depth == sections.size() - 1) {
				handler = it->second.second;
			}
			else {
				handler = it->second.first->handler(sections, depth + 1);
			}
		}

		if (handler) return handler;

		it = m_filters.find("#");
		if (m_filters.end() != it) {
			handler = it->second.second;
		}
	}
	return handler;
}

void CUriFilter::clean()
{
	for (auto& it : m_filters) {
		it.second.first->clean();
		delete it.second.first;
	}
	m_filters.clear();
}

}
