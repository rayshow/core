#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/type_traits/impl/has_member_decl.h>
#include<ccdk/mpl/iterator/algorithm/foreach.h>
#include<ccdk/mpl/iterator/iterator_traits.h>
#include<ccdk/mpl/iterator/iterator_traits.h>

ccdk_namespace_mpl_it_start

//find from front
CCDK_TT_HAS_MEMBER_EXTRA_DECL(has_member_find, find<N>);

//////////////////////////////////////////////////////////////////////////////////////
//// find Times-th matching  iterator which match Prediction

//container version, has find<Times>
template<
	uint32 Times = 1,     //first time appear
	typename Container,
	typename FN,          // Container::value_type => bool 
	typename = check_t< has_member_find< Container, Times, FN> >>
CCDK_FORCEINLINE auto find(Container const& ct, FN Fn) noexcept { 
	return ct.template find<Times>(Fn);
}

//container version, not has find<Times>, use iterator loop instead
template<
	uint32 Times = 1,
	typename Container,
	typename FN,          // Container::value_type => bool 
	typename = check_t<not_<has_member_find<Container,Times, FN>> >,
	typename = check_t<has_nest_iterator<Container>> >
CCDK_FORCEINLINE auto find(Container const& ct, FN Fn) {
	return find(ct.begin(), ct.end(), v);
}

////////////////////////////////////////////////////////////////////////////////////////////
//// iterator version

template<
	uint32 Times = 1,
	typename InputIt,
	typename FN,
	typename = check_t< is_iterator<InputIt>>>
auto find(InputIt begin, InputIt end, FN Fn) {
	uint32 count = 0;
	for (auto it = begin; it != end; ++it) {
		if (Fn(*it)) {
			++count;
			if (count == Times) {
				return it;
			}
		}
	}
	return end;
}

ccdk_namespace_mpl_it_end