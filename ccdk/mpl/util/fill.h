#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/base/logic_.h>
#include<ccdk/mpl/base/arithmetic_.h>
#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/util/addressof.h>
#include<ccdk/mpl/type_traits/is_pod.h>
#include<ccdk/mpl/type_traits/has_assigner.h>

ccdk_namespace_mpl_util_start

/* 2-value move and 3-value pointer move */
#if defined( CCDK_COMPILER_MSVC ) 

//no suitable implements found, for msvc-17+ to  to get detail line and file error place
template< typename T1, typename T2, typename = check_t<false_> >
CCDK_FORCEINLINE void fill(T1* t1, T2& t2, ptr::size_t n) {}

#elif defined( CCDK_COMPILER_GCC )

//no suitable implements found, for gcc  to get detail line and file error place 
template< typename T1, typename T2>
CCDK_FORCEINLINE void fill(T1* t1, T2& t2, ptr::size_t n);

#else //clang or some compiler not found suitable method

template< typename T1, typename T2 >
CCDK_FORCEINLINE void fill(T1* t1, T2& t2, ptr::size_t n)
{
	static_assert(false_::value, "no suitable swap found");
}
#endif

/* pod-byte type use memset to fill */
template<typename T, 
	typename = check_t< and_<is_pod<T>, bool_<sizeof(T) == 1> >>           /* T is pod, and T size == 1 */
>
void fill(T* dest, const T& src, ptr::size_t n)
{
	DebugValue("pod-version");
	memset((void*)dest, (int)*reinterpret_cast<const char*>( addressof(src) ), n);
}


/* non-pod-byte type use assign */
template<typename T,
	typename = check_t< or_< not_<is_pod<T>>, bool_<sizeof(T) != 1> >>, /* T is not pod or sizeof(T) != 1 */
	typename = check_t< has_copy_assigner<T>>                           /* and T have copy assigner */
>
void fill(T* dest, const T& src, ptr::size_t n)
{ 
	DebugValue("non-pod-version");
	for (int i = 0; i < n; ++i) { dest[i] = src; }
}


ccdk_namespace_mpl_util_end