#pragma once
#include"../common_def.h"
#include<cstdlib>
#include<cassert>
namespace Aurora3D
{
	struct GenericString
	{
		template<typename Ch>
		A3D_FORCEINLINE static bool IsValid(Ch)
		{
			return true;
		}

		template<>
		A3D_FORCEINLINE static bool IsValid<achar>(achar ch)
		{
			return ch >= 0 && ch <= 0x7f;
		}

		A3D_FORCEINLINE static uint32 FormatVariant(achar* buffer,
			uint32 count, const achar* fmt, va_list& ap)
		{
			return vsnprintf(buffer, count, fmt, ap);
		}

		A3D_FORCEINLINE static uint32 FormatVariant(wchar* buffer,
			uint32 count, const wchar* fmt, va_list& ap)
		{
			
			return _vsnwprintf(buffer, count, fmt, ap);
		}

		A3D_FORCEINLINE static const achar* FindSequence(const achar* src, const achar* pattern)
		{
			return strstr(src, pattern);
		}

		A3D_FORCEINLINE static const wchar* FindSequence(const wchar* src, const wchar* pattern)
		{
			return wcsstr(src, pattern);
		}

		A3D_FORCEINLINE static size_type Length(const achar* src)
		{
			return strlen(src);
		}

		A3D_FORCEINLINE static size_type Length(const wchar* src)
		{
			return wcslen(src);
		}

		A3D_FORCEINLINE static const achar* FindChar(const achar* src, achar ch)
		{
			return strchr(src, ch);
		}

		A3D_FORCEINLINE static const wchar* FindChar(const wchar* src, wchar ch)
		{
			return wcschr(src, ch);
		}



		//override all base-type conversion
		A3D_FORCEINLINE static achar* FromString(const achar* src, float& F)
		{
			achar* pos = nullptr;
			F = strtof(src, &pos);
			return pos;
		}

		A3D_FORCEINLINE static achar* FromString(const achar* src, double& D)
		{
			achar* pos = nullptr;
			D = strtod(src, &pos);
			return pos;
		}

		A3D_FORCEINLINE static achar* FromString(const achar* src, uint8& U)
		{
			achar *pos = nullptr;
			U = (uint8)strtoul(src, &pos, 10);
			return pos;
		}

		A3D_FORCEINLINE static achar* FromString(const achar* src, int8& I)
		{
			achar* pos = nullptr;
			I = (int8)strtol(src, &pos, 10);
			return pos;
		}

		A3D_FORCEINLINE static achar* FromString(const achar* src, uint16& U)
		{
			achar* pos = nullptr;
			U = (uint16)strtoul(src, &pos, 10);
			return pos;
		}

		A3D_FORCEINLINE static achar* FromString(const achar* src, int16& I)
		{
			achar* pos = nullptr;
			I = (int16)strtol(src, &pos, 10);
			return pos;
		}

		A3D_FORCEINLINE static achar* FromString(const achar* src, uint32& U)
		{
			achar* pos = nullptr;
			U = strtoul(src, &pos, 10);
			return pos;
		}

		A3D_FORCEINLINE static achar* FromString(const achar* src, int32& I)
		{
			achar* pos = nullptr;
			I = strtol(src, &pos, 10);
			return pos;
		}

		A3D_FORCEINLINE static achar* FromString(const achar* src, uint64& U)
		{
			achar* pos = nullptr;
			U = strtoull(src, &pos, 10);
			return pos;
		}

		A3D_FORCEINLINE static achar* FromString(const achar* src, int64& I)
		{
			achar* pos = nullptr;
			I = strtoll(src, &pos, 10);
			return pos;
		}

		A3D_FORCEINLINE static wchar* FromString(const wchar* src, float& F)
		{
			wchar* pos = nullptr;
			F = wcstof(src, &pos);
			return pos;
		}

		A3D_FORCEINLINE static wchar* FromString(const wchar* src, double& D)
		{
			wchar* pos = nullptr;
			D = wcstod(src, &pos);
			return pos;
		}

		A3D_FORCEINLINE static wchar* FromString(const wchar* src, uint8& U)
		{
			wchar* pos = nullptr;
			U = wcstoul(src, &pos, 10);
			return pos;
		}

		A3D_FORCEINLINE static wchar* FromString(const wchar* src, int8& I)
		{
			wchar* pos = nullptr;
			I = wcstol(src, &pos, 10);
			return pos;
		}

		A3D_FORCEINLINE static wchar* FromString(const wchar* src, uint16& U)
		{
			wchar* pos = nullptr;
			U = wcstoul(src, &pos, 10);
			return pos;
		}

		A3D_FORCEINLINE static wchar* FromString(const wchar* src, int16& I)
		{
			wchar* pos = nullptr;
			I = wcstol(src, &pos, 10);
			return pos;
		}

		A3D_FORCEINLINE static wchar* FromString(const wchar* src, uint32& U)
		{
			wchar* pos = nullptr;
			U = wcstoul(src, &pos, 10);
			return pos;
		}

		A3D_FORCEINLINE static wchar* FromString(const wchar* src, int32& I)
		{
			wchar* pos = nullptr;
			I = wcstol(src, &pos, 10);
			return pos;
		}

		A3D_FORCEINLINE static wchar* FromString(const wchar* src, uint64& I)
		{
			wchar* pos = nullptr;
			I = wcstoull(src, &pos, 10);
			return pos;
		}

		A3D_FORCEINLINE static wchar* FromString(const wchar* src, int64& I)
		{
			wchar* pos = nullptr;
			I = wcstoll(src, &pos, 10);
			return pos;
		}
	};

	typedef GenericString PlatformString;

}