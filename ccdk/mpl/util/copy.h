#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/type_traits/is_pod.h>
#include<string.h>

ccdk_namespace_mpl_util_start


#if defined( CCDK_COMPILER_MSVC ) 

//no suitable implements found, for msvc-17+ to  to get detail line and file error place
template< typename T, typename = check_t<false_> >
CCDK_FORCEINLINE T* copy(T, T, ptr::size_t n) { return nullptr; }


#elif defined( CCDK_COMPILER_GCC )

//no suitable implements found, for gcc  to get detail line and file error place 
template< typename T>
CCDK_FORCEINLINE T* copy(T, T, ptr::size_t n);

#else //clang or some compiler not found suitable method

template< typename T>
CCDK_FORCEINLINE T* copy(T, T, ptr::size_t n)
{
	static_assert(false_::value, "no suitable swap found");
	return nullptr;
}
#endif

/* copy n T from src to dest for pod */
template< typename T, typename = check_t< is_pod<T>>>
CCDK_FORCEINLINE T* copy(T* dest, T const* src, ptr::size_t n = 1)
{
	DebugFunctionName();
	static constexpr ptr::size_t type_size = sizeof(T);
	return (T*)memcpy((void*)dest, (void*)src, type_size*n);
}

ccdk_namespace_mpl_util_end
