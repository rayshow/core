#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/base/logic_.h>
#include<ccdk/mpl/type_traits/is_pod.h>
#include<string.h>

ccdk_namespace_mpl_util_start

/* copy n T from src to dest for pod */
template< typename T, typename = check_t< is_pod<T>>>
CCDK_FORCEINLINE void copy(T& dest, T& const* src, ptr::size_t n = 1)
{

}

/* copy n T from src to dest for pod */
template< typename T, typename = check_t< is_pod<T>>>
CCDK_FORCEINLINE void copy(T* dest, T const* src, ptr::size_t n = 1)
{
	DebugFunctionName();
	if (n == 0) return dest;
	memcpy((void*)dest, (void*)src, sizeof(T)*n);
}

/* copy n T from src to dest for pod */
template< typename T, typename = check_t< not_<is_pod<T>>>>
CCDK_FORCEINLINE void copy(T* dest, T const* src, ptr::size_t n = 1)
{ 
	DebugFunctionName();
	if (ccdk_unlikely(n == 0)) return dest;
	for (ptr::size_t i=0; i<n; ++i)
	{
		*(dest + i) = *(src + i);
	}
	return dest;
	
}

ccdk_namespace_mpl_util_end
