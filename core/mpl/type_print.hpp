#pragma once
#include<cstdio>
#include<string>
//#include<core/mpl/type_traits/first_template.h>


template<typename T>
struct TypeNameHelper
{
	std::string operator()()
	{
		return typeid(T).name();
	}
};

template<typename T>
struct TypeNameHelper< const volatile T>
{
	std::string operator()()
	{
		return  std::string{ "const volatile " }+TypeNameHelper<T>{}();
	}
};

template<typename T>
struct TypeNameHelper< const T>
{
	std::string operator()()
	{
		return  std::string{ "const " }+TypeNameHelper<T>{}();
	}
};

template<typename T>
struct TypeNameHelper<volatile T>
{
	std::string operator()()
	{
		return std::string{ "volatile " }+TypeNameHelper<T>{}();
	}
};

template<typename T>
struct TypeNameHelper<T&>
{
	std::string operator()()
	{
		return  std::string{}+TypeNameHelper<T>{}() + " &";
	}
};

template<typename T>
struct TypeNameHelper<T&&>
{
	std::string operator()()
	{
		return  std::string{}+TypeNameHelper<T>{}() + " &&";
	}
};

template<typename T>
struct TypeNameHelper<T*>
{
	std::string operator()()
	{
		return std::string{}+TypeNameHelper<T>{}() + " *";
	}
};

template<typename T>
struct TypeNameHelper<T* const>
{
	std::string operator()()
	{
		return std::string{}+TypeNameHelper<T>{}() + " * const";
	}
};

template<typename T>
struct TypeNameHelper<T* volatile>
{
	std::string operator()()
	{
		return std::string{}+TypeNameHelper<T>{}() + " * volatile";
	}
};

template<typename T>
struct TypeNameHelper<T* const volatile>
{
	std::string operator()()
	{
		return std::string{}+TypeNameHelper<T>{}() + " * const volatile";
	}
};

//template<typename T>
//struct TypeName
//{
//	void operator()()
//	{
//		cout << "      " << left << setw(40) << TypeNameHelper<typename FirstTemplateType<T>::type >{}() << " ==> " << TypeNameHelper<typename T::type >{}() << endl;
//	}
//};

template<typename T>
struct NormalTypeName
{
	void operator()()
	{
		printf("%s\n", TypeNameHelper<T>{}().c_str() );
	}
};
