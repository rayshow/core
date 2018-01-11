#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/type_traits/remove_ref.h>
#include<ccdk/mpl/type_traits/remove_const.h>

ccdk_namespace_mpl_util_start

/*
	compatible with std::move, but can't convert const to right-reference
	T         -> T&&
	T&        -> T&&
	T&&       -> T&&
	const T&  -> const T&&
	const T&& -> const T&&
*/
template<
	typename T,
	typename P = remove_ref_t<T>
> 
CCDK_FORCEINLINE constexpr 
P&& move(T&& inT) noexcept 
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
CCDK_FORCEINLINE constexpr  
P&& fmove(T&& inT) noexcept
{
	return const_cast<T&&>(inT);
}

template< typename T>
CCDK_FORCEINLINE constexpr
T&& fmove(T const& inT) noexcept
{
	return const_cast<T&&>(inT);
}
			

ccdk_namespace_mpl_util_end