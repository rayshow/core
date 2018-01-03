#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/type_traits/remove_ref.h>
#include<ccdk/mpl/type_traits/remove_top_const.h>

ccdk_namespace_mpl_fn_start

// T T& T&& const T&
template<typename T>
CCDK_FORCEINLINE constexpr
T&& const_forward(remove_ref_t<T>& inT) noexcept
{
	return static_cast<T&&>(inT);
}

template<typename T>
CCDK_FORCEINLINE constexpr
T&& const_forward(remove_ref_t<T>&& inT) noexcept
{
	return static_cast<T&&>(inT);
}

template<typename T, typename P = remove_ref_t< remove_top_const_t<T>> >
CCDK_FORCEINLINE constexpr
P&& const_forward(P const& inT) noexcept
{
	return const_cast<P&&>(inT);
}


ccdk_namespace_mpl_fn_end