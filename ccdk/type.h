#pragma once

#include<ccdk/compile.h>
#include<new>       /* nullptr_t and nothrow_t */
#include<typeinfo>  /* std::typeinfo */
#include<cstring>   /* for memory move and copy*/
#include<exception> /* for std exception */

namespace ccdk
{
	/* for short and uniform */
	typedef unsigned char            uint8;
	typedef unsigned short int       uint16;
	typedef unsigned int             uint32;
	typedef unsigned long long       uint64;
	typedef signed char              int8;
	typedef signed short int         int16;
	typedef signed int               int32;
	typedef signed long long         int64;
	typedef long double              ldouble; 
	typedef wchar_t                  wchar;  /* wide char */
	typedef char                     achar;  /* ansi char */
	typedef char16_t                 char16;
	typedef char32_t                 char32;

	static_assert(sizeof(uint8) == 1, "uint8 is not 1 byte.");
	static_assert(sizeof(int8) == 1, "int8 is not 1 byte.");
	static_assert(sizeof(uint16) == 2, "uint16 is not 2 byte.");
	static_assert(sizeof(int16) == 2, "int16 is not 2 byte.");
	static_assert(sizeof(uint32) == 4, "uint32 is not 4 byte.");
	static_assert(sizeof(int32) == 4, "int32 is not 4 byte.");
	static_assert(sizeof(uint64) == 8, "uint64 is not 8 byte.");
	static_assert(sizeof(int64) == 8, "int64 is not 8 byte.");


	/* memory releative type / helpful function */
	namespace ptr
	{
		template<int32 size> struct ptr_traits { static_assert(size != 4 || size != 8, "unkown ptr size."); };
		template<> struct ptr_traits<4> { typedef uint32 size_t;  typedef int32 diff_t; };
		template<> struct ptr_traits<8> { typedef uint64 size_t;  typedef int64 diff_t; };
		typedef typename ptr_traits<sizeof(void*)>::size_t size_t;  
		typedef typename ptr_traits<sizeof(void*)>::diff_t diff_t;
		typedef decltype(nullptr) nullptr_t;
		typedef std::nothrow_t nothrow_t;
		constexpr nothrow_t nothrow;

		template<typename T>
		void safe_delete(T*& t)
		{
			//rise error if T is imcomplete like void*
			sizeof(*t); if (t) { delete t; t = nullptr; }
		}

		template<typename T>
		void safe_delete_array(T*& t)
		{
			//rise error if T is imcomplete
			sizeof(*t); if (t) { delete[] t; t = nullptr; }
		}

		//like void* / declare Type*, not check type
		template<typename T>
		void imcomp_delete(T*& t)
		{
			if (t) { delete t; t = nullptr; }
		}

#define ccdk_safe_delete_if_exception( content , expr )                        \
			try { (content) = (expr); }                                        \
			catch (...) { ptr::safe_delete((content)); throw;  /* rethrow */ } 

#define ccdk_safe_cleanup_if_exception( statement, cleanup )                   \
			try { (statement); }                                               \
			catch (...) { (cleanup); throw; /* rethrow */ }               

#define ccdk_throw_if(expr, except)      if(ccdk_unlikely(expr)){ ccdk_throw(except); }
#define ccdk_check_index(index, length)  ccdk_throw_if( (((index)>=(length) && (index)<=-(int64)(length))), std::out_of_range{"invalid index"} )
#define ccdk_check_expr(expr, exception) ccdk_throw_if(!(expr), exception )

#define ccdk_if_not_this(T)          if(ccdk_likely( ::ccdk::mpl::util::addressof(T)!=this ))
#define ccdk_if_not_same_addr(T1,T2) if(ccdk_likely( ::ccdk::mpl::util::addressof(T1)!=::ccdk::mpl::util::addressof(T2) ))

#define CCDK_TFN(T)  template<typename...> typename T
#define CCDK_TFN1(T) template<typename, typename...> typename T

	}

}

#include<ccdk/debug_type.hpp>


