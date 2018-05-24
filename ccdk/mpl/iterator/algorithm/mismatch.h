#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/type_traits/impl/has_member_decl.h>
#include<ccdk/mpl/iterator/iterator_traits.h>
#include<ccdk/mpl/fusion/pair.h>

ccdk_namespace_mpl_it_start
template<
	typename Container1,
	typename Container2,
	typename FN>
void mismatch(Container1 const& ct1, Container2 const& ct2, FN Fn) {
	return mismatch(ct1.begin(), ct1.end(), ct2.begin(), ct2.end());
}

template<
	typename InputIt1,
	typename InputIt2,
	typename FN,
	typename = check_t< is_iterator<InputIt1>>,
	typename = check_t< is_iterator<InputIt2>> >
void mismatch(InputIt1 begin1, InputIt1 end1, InputIt2 begin2, InputIt2 end2, FN Fn) {
	for (; begin1 != end1, begin2 != end2; ++begin1, ++begin2) {
		if (!Fn(*begin1, *begin2)) {
			break;
		}
	}
	return fs::make_pair(begin1, begin2);
}

ccdk_namespace_mpl_it_end