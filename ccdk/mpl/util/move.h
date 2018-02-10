#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/base/logic_.h>
#include<ccdk/mpl/base/arithmetic_.h>
#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/type_traits/remove_ref.h>
#include<ccdk/mpl/type_traits/remove_const.h>
#include<ccdk/mpl/type_traits/is_pod.h>
#include<ccdk/mpl/type_traits/has_assigner.h>
#include<ccdk/mpl/util/addressof.h>


ccdk_namespace_mpl_util_start

/*
	compatible with std::move,  can't convert const to right-reference
	T         -> T&&
	T&        -> T&&
	T&&       -> T&&
	const T&  -> const T&&
	const T&& -> const T&&
*/
template< typename T, typename P = remove_ref_t<T> > 
CCDK_FORCEINLINE constexpr  P&& move(T&& inT) noexcept 
{ 				
	return static_cast<P&&>(inT); 
}

/*
	force move 
	not compatible with std::move, but can convert const to right-reference
	for some special use
	T         -> T&&
	T&        -> T&&
	T&&       -> T&&
	const T&  -> T&&
	const T&& -> T&&
*/
template< typename T, typename P = remove_ref_t<T>>
CCDK_FORCEINLINE constexpr P&& fmove(T&& inT) noexcept
{
	return static_cast<P&&>(inT);
}

template< typename T>
CCDK_FORCEINLINE constexpr T&& fmove(T const& inT) noexcept
{
	return const_cast<T&&>(inT);
}

/* 2-value move and 3-value pointer move */
#if defined( CCDK_COMPILER_MSVC ) 

//no suitable implements found, for msvc-17+ to  to get detail line and file error place
template< typename T1, typename T2, typename = check_t<false_> >
CCDK_FORCEINLINE void move(T1& t1, T2& t2) {}

#elif defined( CCDK_COMPILER_GCC )

//no suitable implements found, for gcc  to get detail line and file error place 
template< typename T1, typename T2>
CCDK_FORCEINLINE void move(T1& t1, T2& t2);

#else //clang or some compiler not found suitable method

template< typename T1, typename T2 >
CCDK_FORCEINLINE void move(T1& t1, T2& t2)
{
	static_assert(false_::value, "no suitable swap found");
}
#endif


/* has move assigner */
template<typename T, typename = check_t< has_move_assigner<T>> >
void move(T& dest, const T& src)
{
	dest = fmove(src);
}

/* pod-type array*/
template<typename T, ptr::size_t D, ptr::size_t S,
	typename = check_t< is_pod<T>>
>
void move(T(&dest)[D], T(&src)[S])
{
	memcpy((void*)addressof(dest), (void*)addressof(src), sizeof(T)*min_val<ptr::size_t, D, S>);
}

/* pod-type array*/
template<typename T, ptr::size_t D, ptr::size_t S,
	typename = check_t< not_<is_pod<T>>>,          /* T is not pod  */
	typename = check_t< has_move_assigner<T>>      /* and has move asssigner */
>
void move(T(&dest)[D], T(&src)[S])
{ 
	for (int i = 0; i < min_val<ptr::size_t, D, S>; ++i) { dest[i] = fmove(src[i]); }
}

/* pod type array  */
template<typename T, typename = check_t< is_pod<T>> >
CCDK_FORCEINLINE void move(T* dest, const T* src, ptr::size_t n )
{
	if (ccdk_unlikely(n == 0 || dest == src)) return;
	memmove((void*)dest, (void*)src, sizeof(T)*n);
}

/* non-pod type array move , need avoid override overlapped aera*/
template<typename T,
	typename = check_t< not_<is_pod<T>>>,          /* T is not pod  */
	typename = check_t< has_move_assigner<T>>      /* and has move asssigner */
>
CCDK_FORCEINLINE void move(T* dest, const T* src, ptr::size_t n)
{
	if (ccdk_unlikely(n == 0 || dest == src)) return;
	if (dest > src) {  for (ptr::size_t i = n - 1; i != 0; --i)  { *(dest + i) = fmove(*(src + i)); }  }
	else if (dest < src) { for (ptr::size_t i = 0; i < n; ++i) { *(dest + i) = fmove(*(src + i)); } }
}

ccdk_namespace_mpl_util_end