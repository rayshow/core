#pragma once

#include<ccdk/mpl/mpl_module.h>

ccdk_namespace_mpl_util_start


/* trivially destructor */
template<typename T>
CCDK_FORCEINLINE static constexpr void destruct(void* memory)
{
	reinterpret_cast<T*>(memory)->~T();
}

ccdk_namespace_mpl_util_end