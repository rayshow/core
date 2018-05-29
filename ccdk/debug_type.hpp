#pragma once

#include<iostream>
#include<string>
#include<cassert>
#include<ccdk/compile.h>
#include<ccdk/type.h>

namespace ccdk {

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
#define RuntimeAssertFalse(v) assert(!(v))

#if !defined(CCDK_NO_TRACE)
#define DebugNewTitle(T)  std::cout<<"\n ================ " << T << " ================" << std::endl;
#define DebugSubTitle(T)  std::cout<<"\n --- " << T << " --- " << std::endl;
#else
#define DebugNewTitle(T)
#define DebugSubTitle(T)
#endif
	//#define DebugValue(v)     std::cout<<" *** " << v <<std::endl;
	//#define DebugTypeName(T) 

	template<typename T>
	inline void DebugValue(T&& t)
	{
#if !defined(CCDK_NO_TRACE)
		std::cout << " *** " << t << std::endl;
#endif
	}

	template<typename T>
	inline void DebugValueItBegin(T&& t)
	{
#if !defined(CCDK_NO_TRACE)
		std::cout << t << ": ";
#endif
	}

	template<typename T>
	inline void DebugValueIt(T&& t)
	{
#if !defined(CCDK_NO_TRACE)
		std::cout << t <<" ";
#endif
	}

	template<typename T, typename P>
	inline void DebugValueIt(T&& t, P&& p)
	{
#if !defined(CCDK_NO_TRACE)
		std::cout << t << " "<<p<<" ";
#endif
	}

	inline void DebugValueItEnd()
	{
#if !defined(CCDK_NO_TRACE)
		std::cout << std::endl;
#endif
	}



	template<typename T, typename P>
	inline void DebugValue(T&& t, P&& p)
	{
#if !defined(CCDK_NO_TRACE)
		std::cout << " *** " << t <<"  "<< p << std::endl;
#endif
	}

	template<typename T>
	inline void DebugTypeName()
	{
#if !defined(CCDK_NO_TRACE)
		std::cout << " *** typename: " << TypeNameHelper<T>{}() << std::endl;
#endif
	}

	template<typename T>
	inline void DebugValueTypeName(T&& v)
	{
#if !defined(CCDK_NO_TRACE)
		DebugTypeName<decltype(v)>();
#endif
	}

	template<typename T>
	inline void DebugTypeAndValue(T&& v)
	{
#if !defined(CCDK_NO_TRACE)
		std::cout << " *** typename: " << TypeNameHelper<decltype(v)>{}() << " -- value: " << v << std::endl;
#endif
	}





	// if T& version constructor not exists, will call const T& version
	struct test_copy_t
	{
		test_copy_t(int val) :v(val) { DebugValue("value ctor"); }
		test_copy_t() { DebugValue("default ctor"); }
		test_copy_t(test_copy_t&& t):v(t.v) { DebugValue("move ctor"); }
		test_copy_t(test_copy_t& t) :v(t.v) { DebugValue("copy ctor"); }
		test_copy_t(const test_copy_t& t) :v(t.v) { DebugValue("const copy ctor"); }

		void operator=(test_copy_t& t) { DebugValue("copy assign"); }
		void operator=(test_copy_t&& t) { DebugValue("move assign"); }
		void operator=(const test_copy_t& t) { DebugValue("const copy assign"); }

		~test_copy_t() { DebugValue("destroy"); }

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

		operator int() {
			return v;
		}

		int v;
	};
	struct test_constexpr_t
	{
		int test_mfn(int a, const char* msg)
		{
			DebugValue(msg);
			return a;
		}
	};


	template<typename T>
	void type_copy(T* dest, const T* src, uint32 len)
	{
		for (uint32 i = 0; i < len; ++i)
		{
			dest[i] = src[i];
		}
	}

	int char_len(const char* src)
	{
		int len = 0;
		while (*(src++) != 0) ++len;
		return len;
	}

	struct test_destruct
	{
		char * val;
		int size;

		test_destruct(const char* inVal)
		{
			size = char_len(inVal);
			val = new char[size + 1];
			val[size] = 0;
			type_copy(val, inVal, size);
		}

		test_destruct(const test_destruct& other)
			: size{ other.size }
			, val{ new char[other.size + 1] }
		{
			val[size] = 0;
			type_copy(val, other.val, size);
		}

		~test_destruct()
		{
			if (val)
			{
				DebugValue("destructing test_destruct of val:", val);
			}
			
			ptr::safe_delete_array(val);
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

		test_copy_assign_t(int t) { DebugValue("int ctor"); }

		test_copy_assign_t(test_copy_assign_t& t) { DebugValue("copy ctor"); }

		test_copy_assign_t(test_copy_assign_t&& t) { DebugValue("move ctor"); }

		test_copy_assign_t(const test_copy_assign_t& t) { DebugValue("const copy ctor"); }

		template<typename U>
		test_copy_assign_t(test_copy_assign_t<U>& t) { DebugValue("tmpl copy ctor"); }

		template<typename U>
		test_copy_assign_t(test_copy_assign_t<U>&& t) { DebugValue("tmpl move ctor"); }

		template<typename U>
		test_copy_assign_t(const test_copy_assign_t<U>& t) { DebugValue(" tmpl const copy ctor"); }

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

}
