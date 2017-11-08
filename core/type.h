#pragma once


namespace Aurora3D
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

	static_assert(sizeof(uint8) == 1, "uint8 is not 1 byte.");
	static_assert(sizeof(int8) == 1, "int8 is not 1 byte.");
	static_assert(sizeof(uint16) == 2, "uint16 is not 2 byte.");
	static_assert(sizeof(int16) == 2, "int16 is not 2 byte.");
	static_assert(sizeof(uint32) == 4, "uint32 is not 4 byte.");
	static_assert(sizeof(int32) == 4, "int32 is not 4 byte.");
	static_assert(sizeof(uint64) == 8, "uint64 is not 8 byte.");
	static_assert(sizeof(int64) == 8, "int64 is not 8 byte.");
	
}


