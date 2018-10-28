#include "varianttool/any.h"

namespace varianttool
{

CAny::CAny()
	: m_tpIndex(std::type_index(typeid(void)))
{
}

CAny::CAny(CAny& other)
	: m_ptr(other.clone()), m_tpIndex(other.m_tpIndex)
{

}

CAny::CAny(CAny&& other)
	: m_ptr(std::move(other.m_ptr)), m_tpIndex(std::move(other.m_tpIndex))
{
}

CAny& CAny::operator=(const CAny& other)
{
	if (m_ptr == other.m_ptr)
	{
		return *this;
	}
	m_ptr = other.clone();
	m_tpIndex = other.m_tpIndex;
	return *this;
}

bool CAny::isNull()
{
	return !bool(m_ptr);
}

CAny::BasePtr CAny::clone() const
{
	if (m_ptr != nullptr)
	{
		return m_ptr->clone();
	}
	return nullptr;
}

}
