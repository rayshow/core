#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/base/logic_.h>
#include<ccdk/mpl/base/arithmetic_.h>
#include<ccdk/mpl/type_traits/is_pod.h>
#include<ccdk/mpl/type_traits/is_scalar.h>
#include<ccdk/mpl/type_traits/is_array.h>
#include<ccdk/mpl/type_traits/has_assigner.h>
#include<ccdk/mpl/util/addressof.h>
#include<cstring>

ccdk_namespace_mpl_util_start

/* 2-value copy and 3-value  copy */
#if defined( CCDK_COMPILER_MSVC ) 

/* no suitable implements found, for msvc-17+ to  to get detail line and file error place */
template< typename T1, typename T2, typename = check_t<false_> >
CCDK_FORCEINLINE void copy(T1& t1, const T2& t2) {}
template< typename T1, typename T2, typename T3, typename = check_t<false_> >
CCDK_FORCEINLINE void copy(T1& t1, const T2& t2, ptr::size_t) {}

#elif defined( CCDK_COMPILER_GCC )

/* no suitable implements found, for gcc  to get detail line and file error place  */
template< typename T1, typename T2>
CCDK_FORCEINLINE void copy(T1& t1, const T2& t2);
template< typename T1, typename T2, typename T3>
CCDK_FORCEINLINE void copy(T1& t1, const T2& t2, T3);

#else /* clang or some compiler not found suitable method */

template< typename T1, typename T2 >
CCDK_FORCEINLINE void copy(T1& t1, const T2& t2) { static_assert(false_::value, "no suitable swap found"); }
template< typename T1, typename T2, typename T3 >
CCDK_FORCEINLINE void copy(T1& t1, const T2& t2) { static_assert(false_::value, "no suitable swap found"); }

#endif

/* 2-value copy assign */
template<typename T, typename = check_t< has_copy_assigner<T>> >
CCDK_FORCEINLINE void copy(T& dest, const T& src) noexcept
{
	dest = src;
}

/* pod-type array copy*/
template<typename T, ptr::size_t D, ptr::size_t S,
	typename = check_t< is_pod<T>>
>
CCDK_FORCEINLINE void copy(T(&dest)[D], const T(&src)[S]) noexcept
{
	static constexpr ptr::size_t Min = min_val<ptr::size_t, D, S>;
	memcpy((void*)addressof(dest), (void*)addressof(src), sizeof(T)*Min);
}

/* non-pod-type array copy*/
template<typename T, ptr::size_t D, ptr::size_t S,
	typename = check_t< not_<is_pod<T>>>,          /* T is not pod  */
	typename = check_t< has_copy_assigner<T>>      /* and has copy asssigner */
>
CCDK_FORCEINLINE void copy(T(&dest)[D], const T(&src)[S])
{
	static constexpr ptr::size_t Min = min_val<ptr::size_t, D, S>;
	for (int i = 0; i < Min; ++i) { dest[i] = src[i]; }
}

/* copy n T from src to dest for pod */
template< typename T, 
	typename = check_t< is_pod<T>>  
>
CCDK_FORCEINLINE void copy(T* dest, T const* src, ptr::size_t n)
{
	if (n == 0) return;
	memcpy((void*)dest, (void*)src, sizeof(T)*n);
}

/* copy n T from src to dest for pod */
template< typename T, 
	typename = check_t< not_<is_pod<T>>>,          /* T is not pod  */
	typename = check_t< has_copy_assigner<T>>      /* and has copy assigner */
>
CCDK_FORCEINLINE void copy(T* dest, T const* src, ptr::size_t n)
{
	if (ccdk_unlikely(n == 0 || src == dest)) return;
	for (ptr::size_t i=0; i<n; ++i) { *(dest + i) = *(src + i); }
}


ccdk_namespace_mpl_util_end
