#pragma once
#include<iostream>
#include<string>
#include<cassert>
#include<ccdk/compile.h>



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

#if  defined(CCDK_COMPILER_MSVC)
#define DebugFunctionName()  std::cout << " *** function name: " << __FUNCSIG__ <<std::endl;
#else
#define DebugFunctionName()  std::cout << " *** function name: " << __PRETTY_FUNCTION__ <<std::endl;
#endif

#define AssertTrue(v)  static_assert(v, "");
#define AssertFalse(v) static_assert(!v, "");
#define RuntimeAssertTrue(v) assert(v)
#define DebugNewTitle(T)  std::cout<<"\n ================ " << T << " ================" << std::endl;
#define DebugSubTitle(T)  std::cout<<"\n --- " << T << " --- " << std::endl;
#define DebugValue(v)     std::cout<<" *** " << v <<std::endl;
#define DebugTypeName(T) std::cout<<" *** typename: " << TypeNameHelper<T>{}()<< std::endl;
#define DebugValueTypeName(V) DebugTypeName(decltype(V));
#define DebugValueTypeAndValue(V) std::cout<<" *** typename: " << TypeNameHelper<decltype(V)>{}() << " -- value: "<< V << std::endl;