#pragma once
#include<cstring>
#include"../common_def.h"
namespace Aurora3D
{
	struct GenericMemory
	{
		A3D_FORCEINLINE static void Zero(void *dest, size_type size)
		{
			memset(dest, 0, size);
		}

		A3D_FORCEINLINE static void Clear(void* dest, uint8 val, size_type size)
		{
			memset(dest, val, size);
		}

		A3D_FORCEINLINE static void Copy(void* dest, const void* src, size_type size)
		{
			memcpy(dest, src, size);
		}

		A3D_FORCEINLINE static void Move(void* dest, const void* src, size_type size)
		{
			memmove(dest, src, size);
		}

		A3D_FORCEINLINE static void* Alloc(size_type size)
		{
			return malloc(size);
		}

		A3D_FORCEINLINE static void* Realloc(void* dest, size_type size)
		{
			return realloc(dest, size);
		}
	};

	typedef GenericMemory PlatformMemory;

}