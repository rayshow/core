#pragma once
#include<ccdk/mpl/mpl_module.h>

ccdk_namespace_mpl_util_start

/* dummy call */
template<typename... Args> CCDK_FORCEINLINE constexpr void  dummy_call(Args&&... args) {}

ccdk_namespace_mpl_util_end