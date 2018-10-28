#ifndef __VARIANT_ANY_H__
#define __VARIANT_ANY_H__

#include <memory>
#include <typeindex>

namespace varianttool
{

class CAny
{
public:
	CAny();
	CAny(CAny& other);
	CAny(CAny&& other);
    template<typename U, 
        class = typename std::enable_if<!std::is_same<typename std::decay<U>::type, CAny>::value, U>::type>
    CAny(U&& value)
        : m_ptr(new Derived<typename std::decay<U>::type>(std::forward<U>(value)))
        , m_tpIndex(typeid(typename std::decay<U>::type)) {}
	CAny& operator=(const CAny& other);

public:
	bool isNull();
    template<typename T>
    bool is()
    {
        return m_tpIndex == std::type_index(typeid(T));
    }
    template<typename T>
    T& cast()
    {
        if (!is<T>())
        {
            throw std::bad_cast();
        }
        auto derived = dynamic_cast<Derived<T>*>(m_ptr.get());
        return derived->m_value;
    }

private:
    struct Base;
    using BasePtr = std::unique_ptr<Base>;
    struct Base
    {
        virtual BasePtr clone() const = 0;
    }; 

    template<typename T>
    struct Derived : public Base
    {
        template<typename...Args>
        Derived(Args&& ...args) : m_value(std::forward<Args>(args)...){}
        BasePtr clone() const
        {
            return BasePtr(new Derived(m_value));
        }

        T   m_value;
    };

	BasePtr clone() const;

private:
    BasePtr m_ptr;
    std::type_index m_tpIndex;
};

}


#endif // __VARIANT_ANY_H__
