#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/type_traits/impl/has_member_decl.h>
#include<ccdk/mpl/iterator/algorithm/foreach.h>
#include<ccdk/mpl/iterator/iterator_traits.h>

ccdk_namespace_mpl_it_start

//////////////////////////////////////////////////////////////////////////////////////
//// count
template<
	typename Container,
	typename FN,
	typename = check_t< has_member_foreach< Container> >>
	CCDK_FORCEINLINE ptr::size_t count(Container const& ct, FN Fn) noexcept {
	ptr::size_t cnt = 0;
	ct.foreach([](auto const& v) { if (Fn(v)) ++cnt; });
	return cnt;
}

//container version, don't has foreach
template<
	typename Container, 
	typename FN,
	typename = check_t<not_<has_member_foreach<Container>> >,
	typename = check_t<has_nest_iterator<Container>> >
	CCDK_FORCEINLINE ptr::size_t count(Container const& ct, FN Fn) {
	return count(ct.begin(), ct.end(), Fn);
}

//////////////////////////////////////////////////////////////////////////////////////
//// iterator version

//iterator range version
template<
	typename InputIt, 
	typename FN,
	typename = check_t< is_iterator<InputIt>> >
	CCDK_FORCEINLINE ptr::size_t count(InputIt begin, InputIt end, FN Fn) {
	ptr::size_t cnt = 0;
	for (auto it = begin; it != end; ++it) {
		if (Fn(*it)) ++cnt;
	}
	return cnt;
}


ccdk_namespace_mpl_it_end