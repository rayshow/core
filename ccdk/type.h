#pragma once

#include<ccdk/compile.h>

namespace ccdk
{
	//for short and clear
	typedef unsigned char            uint8;
	typedef unsigned short int       uint16;
	typedef unsigned int             uint32;
	typedef unsigned long long       uint64;
	typedef signed char              int8;
	typedef signed short int         int16;
	typedef signed int               int32;
	typedef signed long long         int64;
	typedef long double              hfloat; 
	typedef wchar_t                  wchar;  //wide char
	typedef char                     achar;  //ansi char
	typedef char16_t                 char16;
	typedef char32_t                 char32;

	namespace ptr
	{
		template<int32 size> struct ptr_traits { static_assert(size != 4 || size != 8, "unkown ptr size."); };
		template<> struct ptr_traits<4> { typedef uint32 size_t;  typedef int32 diff_t; };
		template<> struct ptr_traits<8> { typedef uint64 size_t;  typedef int64 diff_t; };

		typedef typename ptr_traits<sizeof(void*)>::size_t size_t;
		typedef typename ptr_traits<sizeof(void*)>::diff_t diff_t;
		typedef decltype(nullptr) nullptr_t;
	}

	static_assert(sizeof(uint8) == 1, "uint8 is not 1 byte.");
	static_assert(sizeof(int8) == 1, "int8 is not 1 byte.");
	static_assert(sizeof(uint16) == 2, "uint16 is not 2 byte.");
	static_assert(sizeof(int16) == 2, "int16 is not 2 byte.");
	static_assert(sizeof(uint32) == 4, "uint32 is not 4 byte.");
	static_assert(sizeof(int32) == 4, "int32 is not 4 byte.");
	static_assert(sizeof(uint64) == 8, "uint64 is not 8 byte.");
	static_assert(sizeof(int64) == 8, "int64 is not 8 byte.");
}


