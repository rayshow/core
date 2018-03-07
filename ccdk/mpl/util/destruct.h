#pragma once

#include<ccdk/mpl/mpl_module.h>

ccdk_namespace_mpl_util_start


/* trivially destructor */
template<typename T>
CCDK_FORCEINLINE static constexpr void destruct(void* memory)
{
	reinterpret_cast<T*>(memory)->~T();
}

template<typename T>
CCDK_FORCEINLINE static constexpr void destruct_n(void* memory, ptr::size_t n)
{
	if (n == 0) return;
	T* start = reinterpret_cast<T*>(memory);
	for (int i = 0; i < n; ++i, ++start) start->~T();
}

ccdk_namespace_mpl_util_end