#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/type_traits/impl/has_member_decl.h>
#include<ccdk/mpl/iterator/iterator_traits.h>

ccdk_namespace_mpl_it_start

CCDK_TT_HAS_MEMBER_WITH_RET_DECL(has_member_foreach, foreach, void);

// if Container.foreach(Fn) is exists, just use the most efficient version 
template<
	typename Container, typename FN, 
	typename = check_t<has_member_foreach<Container, FN>>>
CCDK_FORCEINLINE void foreach(Container const& ct, FN Fn) { ct.foreach(Fn); }


// Container member-fn foreach not exists but [cbegin,cend) exists , loop over const_iterator
template<
	typename Container, typename FN,
	typename = check_t<not_<has_member_foreach<Container, FN>> >,
	typename = check_t<has_nest_iterator<Container>> >
CCDK_FORCEINLINE void foreach(Container const& ct, FN Fn) {
	foreach(ct.begin(), ct.end(), Fn);
}

//range-version
template<
	typename InputIt, typename FN,
	typename = check_t<is_iterator<InputIt>>>
CCDK_FORCEINLINE void foreach(InputIt begin, InputIt end, FN Fn) {
	for (auto it = begin; it != end; ++it) {
		Fn(*it);
	}
}

ccdk_namespace_mpl_it_end