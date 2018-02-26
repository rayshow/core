#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/util/forward.h>

ccdk_namespace_mpl_util_start

template<typename T, typename... Args>
CCDK_FORCEINLINE static constexpr void construct(void* memory, Args&& ... args)
{
	new(memory) T(util::forward<Args>(args)...);
}

ccdk_namespace_mpl_util_end