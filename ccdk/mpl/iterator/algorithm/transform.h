#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/util/move.h>
#include<ccdk/mpl/type_traits/impl/has_member_decl.h>
#include<ccdk/mpl/iterator/algorithm/foreach.h>

ccdk_namespace_mpl_it_start

template<
	typename Container2,
	typename Container1>
CCDK_FORCEINLINE auto transform(Container1 const& ct) {
	Container2 target{ ct.size() };
	foreach(ct, [](auto const& val) {
		target.push_back(val);
	});
	return target;
}

template<
	typename Container2,
	typename Container1>
	CCDK_FORCEINLINE auto transform(Container1 && ct) {
	Container2 target{ ct.size() };
	foreach(ct, [](auto const& val) {
		target.push_back(util::fmove(val));
	});
	return target;
}

template<typename InputIt1, typename InputIt2, typename FN>
CCDK_FORCEINLINE void transform(InputIt1 begin, InputIt1 end, InputIt2 dest, FN Fn) {
	auto it2 = dest;
	for (auto it = begin; it != end; ++it, ++it2) {
		*it2 = Fn(*it);
	}
}

ccdk_namespace_mpl_it_end