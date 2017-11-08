#pragma once

#include<cstring>
#include"../type.h"


namespace Aurora3D
{
	template<typename T>
	struct TCharTrait
	{
		static constexpr bool IsCharType = false;
	};

	template<>
	struct TCharTrait<achar>
	{
		static constexpr achar End = '\0';
		static constexpr bool IsCharType = true;

		static size_t length(const achar* str)
		{
			return strlen(str);
		}

		static bool equal(const achar* str1, const achar* str2, size_t len)
		{
			return memcmp(str1, str2, len);
		}

		static achar* strncpy( achar* str1, const achar* str2, size_t len)
		{
			return (achar*)memcpy(str1, str2, len);
		}

		//static bool snprintf(const achar* buffer, arg)
	};

	struct TCharTrait<wchar>
	{
		static constexpr wchar End = L'\0';
		static constexpr bool IsCharType = true;
	};

}
