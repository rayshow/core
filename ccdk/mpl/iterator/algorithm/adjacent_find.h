#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/type_traits/impl/has_member_decl.h>
#include<ccdk/mpl/iterator/algorithm/foreach.h>
#include<ccdk/mpl/iterator/iterator_traits.h>
#include<ccdk/mpl/iterator/iterator_traits.h>

ccdk_namespace_mpl_it_start

//////////////////////////////////////////////////////////////////////////////////////
//// find Times-th matching  iterator which is equal to a value

//container version, not has find<Times>, use iterator loop instead
template<
	uint32 Times = 1,
	typename Container,
	typename FN,          // ValueType,ValueType => bool 
	typename = check_t<has_nest_iterator<Container>> >
	CCDK_FORCEINLINE auto adjacent_find(Container const& ct, FN Fn) {
	return adjacent_find(ct.begin(), ct.end(), Fn);
}

////////////////////////////////////////////////////////////////////////////////////////////
//// iterator version

template<
	uint32 Times = 1,
	typename ForwardIt,
	typename FN,
	typename = check_t< is_iterator<ForwardIt>>>
	auto adjacent_find(ForwardIt begin, ForwardIt end, FN Fn) {
	uint32 count = 0;
	auto preit = begin;
	auto it = ++preit;
	for (; it != end; preit = it, ++it) {
		if (Fn(*preit, *it)) {
			++count;
			if (count == Times) {
				return preit;
			}
		}
	}
	return end;
}

ccdk_namespace_mpl_it_end