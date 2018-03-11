#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/type_traits/declval.h>
ccdk_namespace_mpl_util_start

template<typename T>
struct has_member_data
{
	template<typename P, typename = decltype( declval<P>().data() )>
	bool sfinae(int) { return true; }
	template<typename P>
	bool sfinae(...) { return false; }
	static constexpr bool value = sfinae<T>(0);
};


/* default call size(), need check has function T::size() */
template<typename Container, typename = check_t< has_member_data<Container>  >>
CCDK_FORCEINLINE decltype(auto) data(Container const& ct) noexcept { 
	return ct.data(); 
}

ccdk_namespace_mpl_util_end
