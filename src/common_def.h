#pragma once

#include"precompile_def.h"
#include<string>

namespace Aurora3D
{
	template<typename bits32, typename bits64, int pointer_size>
	struct SelectPointerType{};

	template<typename bits32, typename bits64>
	struct SelectPointerType<bits32, bits64, 8>{
		typedef bits64 Type;
	};

	template<typename bits32, typename bits64>
	struct SelectPointerType<bits32, bits64, 4>{
		typedef bits32 Type;
	};
	
	//Generic type platform use
	typedef unsigned char            uint8;
	typedef unsigned short int       uint16;
	typedef unsigned int             uint32;
	typedef unsigned long long       uint64;
	typedef signed char              int8;
	typedef signed short int         int16;
	typedef signed int               int32;
	typedef signed long long         int64;
	typedef wchar_t                  wchar;  //wide char
	typedef char                     achar;  //ansi char
	typedef char16_t				 char16; //ucs2
	typedef char32_t                 char32; //utf-32
	typedef std::string              AString;
	typedef std::wstring             WString;
	typedef std::u16string           U16String;
	typedef std::u32string           U32String;

	typedef SelectPointerType<int32,  int64, sizeof(void*)>::Type   int_deps;
	typedef SelectPointerType<uint32, uint64, sizeof(void*)>::Type uint_deps;
	typedef uint_deps                size_type;
	typedef int_deps                 diff_type;

	static_assert(sizeof(uint8) == 1, "uint8 is not 1 byte.");
	static_assert(sizeof(int8) == 1, "int8 is not 1 byte.");
	static_assert(sizeof(uint16) == 2, "uint16 is not 2 byte.");
	static_assert(sizeof(int16) == 2, "int16 is not 2 byte.");
	static_assert(sizeof(uint32) == 4, "uint32 is not 4 byte.");
	static_assert(sizeof(int32) == 4, "int32 is not 4 byte.");
	static_assert(sizeof(uint64) == 8, "uint64 is not 8 byte.");
	static_assert(sizeof(int64) == 8, "int64 is not 8 byte.");

	//return status
	struct Status
	{
		unsigned code = 0;
		std::string msg{};
		bool Ok(){ return 0 == code; }
		const static int InitialFailed = 1;
		const static int FatalError = 2;
	};


}


