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
//#define DebugValue(v)     std::cout<<" *** " << v <<std::endl;
//#define DebugTypeName(T) 

template<typename T>
inline void DebugValue(T&& t)
{
	std::cout << " *** " << t << std::endl;
}

template<typename T>
inline void DebugTypeName()
{
	std::cout << " *** typename: " << TypeNameHelper<T>{}() << std::endl;
}

template<typename T>
inline void DebugValueTypeName(T&& v)
{
	DebugTypeName<decltype(v)>();
}

template<typename T>
inline void DebugTypeAndValue(T&& v)
{
	std::cout << " *** typename: " << TypeNameHelper<decltype(v)>{}() << " -- value: " << v << std::endl;
}




// if T& version constructor not exists, will call const T& version
struct test_copy_t
{
	test_copy_t() { DebugValue("default ctor"); }
	test_copy_t(test_copy_t&& t) { DebugValue("move ctor"); }
	test_copy_t(const test_copy_t& t) { DebugValue("const copy ctor"); }

	void operator=(test_copy_t& t) { DebugValue("copy assign"); }
	void operator=(test_copy_t&& t) { DebugValue("move assign"); }
	void operator=(const test_copy_t& t) { DebugValue("const copy assign"); }

	int operator()(int a, const char* msg)
	{
		DebugValue(msg);
		return a;
	}

	int test_mfn(int a, const char*msg)
	{
		DebugValue(msg);
		return a;
	}
};
struct test_constexpr_t
{
	int test_mfn(int a, const char*msg)
	{
		DebugValue(msg);
		return a;
	}
};



int test_normal_copy_function(const test_copy_t& t, const char* msg)
{
	DebugValue(msg);
	return 0;
}

int test_normal_move_function(test_copy_t&& t, const char* msg)
{
	DebugValue(msg);
	return 0;
}

template<typename T>
struct test_copy_assign_t
{
	test_copy_assign_t() { DebugValue("default ctor"); }

	test_copy_assign_t(test_copy_assign_t& t) { DebugValue("copy ctor"); }

	test_copy_assign_t(test_copy_assign_t&& t) { DebugValue("move ctor"); }

	test_copy_assign_t(const test_copy_assign_t& t) { DebugValue("const copy ctor"); }

	template<typename U>
	test_copy_assign_t(test_copy_assign_t<U>& t) { DebugValue("tmpl copy ctor"); }

	template<typename U>
	test_copy_assign_t(test_copy_assign_t<U>&& t) { DebugValue("tmpl move ctor"); }

	template<typename U>
	test_copy_assign_t(const test_copy_assign_t<U>& t) { DebugValue("2 tmpl const copy ctor"); }

	void operator=(test_copy_assign_t& t) { DebugValue("copy assign"); }

	void operator=(test_copy_assign_t&& t) { DebugValue("move assign"); }

	void operator=(const test_copy_assign_t& t) { DebugValue("const copy assign"); }

	template<typename U>
	void operator=(test_copy_assign_t<U>& t) { DebugValue("tmpl copy assign"); }

	template<typename U>
	void operator=(test_copy_assign_t<U>&& t) { DebugValue("tmpl move assign"); }

	template<typename U>
	void operator=(const test_copy_assign_t<U>& t) { DebugValue("tmpl const copy assign"); }
};
