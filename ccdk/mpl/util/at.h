#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/type_traits/declval.h>

ccdk_namespace_mpl_util_start

template<typename T>
struct has_member_at
{
	template<typename P, typename = decltype(declval<P>().at())>
	bool sfinae(int) { return true; }
	template<typename P>
	bool sfinae(...) { return false; }
	static constexpr bool value = sfinae<T>(0);
};

/* default call size(), need check has function T::at() */
template<typename T, typename = check_t< has_member_at<T>>  >
CCDK_FORCEINLINE decltype(auto) at(T const& t, ptr::size_t index) noexcept { 
	return t.at(index); 
}

ccdk_namespace_mpl_util_end