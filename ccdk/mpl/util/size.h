#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/type_traits/declval.h>
ccdk_namespace_mpl_util_start

template<typename T>
struct has_member_size
{
	template<typename P, typename = decltype( declval<P>().size() )>
	bool sfinae(int) { return true; }
	template<typename P>
	bool sfinae(...) { return false; }
	static constexpr bool value = sfinae<T>(0);
};

/* default call size(), need check has function T::size() */
template<typename T, typename = check_t< has_member_size<T>> >
CCDK_FORCEINLINE void size(const T& t) noexcept { return t.size(); }

template<typename T, typename = check_t< has_member_size<T>> >
CCDK_FORCEINLINE bool empty(const T& t) noexcept { return t.size()==0; }

ccdk_namespace_mpl_util_end
