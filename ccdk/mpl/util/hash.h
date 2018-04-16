#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/util/move.h>

ccdk_namespace_mpl_util_start


// FNV-1a hash function for bytes in [bytes, bytes + n)
inline ptr::size_t _Hash_bytes(const uint8 *bytes, ptr::size_t n) _NOEXCEPT
{	
#if defined(_WIN64)
	static_assert(sizeof(ptr::size_t) == 8, "This code is for 64-bit size_t.");
	constexpr ptr::size_t kOffsetBasic = 14695981039346656037ULL;
	constexpr ptr::size_t kPrime = 1099511628211ULL;
#else 
	static_assert(sizeof(ptr::size_t) == 4, "This code is for 32-bit size_t.");
	constexpr ptr::size_t kOffsetBasic = 2166136261U;
	constexpr ptr::size_t kPrime = 16777619U;
#endif

	ptr::size_t ret = kOffsetBasic;
	for (ptr::size_t i = 0; i < n; ++i)
	{
		ret ^= (ptr::size_t)bytes[i];
		ret *= kPrime;
	}
	return (ret);
}

ccdk_namespace_mpl_util_end