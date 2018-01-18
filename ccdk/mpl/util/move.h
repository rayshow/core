#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/type_traits/remove_ref.h>
#include<ccdk/mpl/type_traits/remove_const.h>
#include<ccdk/mpl/type_traits/is_pod.h>
#include<ccdk/mpl/util/addressof.h>
#include<string.h>

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

/* pod type */
template<typename T, typename = check_t< is_pod<T>> >
void move(T& dest, T& src)
{
	memmove((void*)addressof(dest), (void*)addressof(src));
}

/* non-pod type */
template<typename T, typename = check_t< not_<is_pod<T>>> >
void move(T& dest, T& src)
{
	dest = move(src);
}


/* pod type array  */
template<typename T, typename = check_t< is_pod<T>> >
void move(T* dest, T* src, ptr::size_t n = 1)
{
	if (ccdk_unlikely(n == 0)) return dest;
	memmove((void*)dest, (void*)src, sizeof(T)*n);
}

/* non-pod type array */
template<typename T, typename = check_t< not_<is_pod<T>>> >
void move(T* dest, T* src, ptr::size_t n = 1)
{
	if (ccdk_unlikely(n == 0)) return dest;
	for (ptr::size_t i = n - 1; i >= 0; --i)
	{
		*(dest + i) = move(*(src + i));
	}
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
	return const_cast<T&&>(inT);
}

template< typename T>
CCDK_FORCEINLINE constexpr T&& fmove(T const& inT) noexcept
{
	return const_cast<T&&>(inT);
}




ccdk_namespace_mpl_util_end