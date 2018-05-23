#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/util/move.h>
#include<ccdk/mpl/type_traits/impl/has_member_decl.h>
#include<ccdk/mpl/iterator/algorithm/foreach.h>

ccdk_namespace_mpl_it_start

template<typename Container2, typename Container1>
CCDK_FORCEINLINE Container2 unique(Container1 const& ct) {
	Container2 ct2{};
	ct2.push_back(ct.front());
	foreach(ct, [](auto const& val) {
		if (ct2.back() != val) {
			ct2.push_back(val);
		}
	});
	return ct2;
}

template<typename ForwardIt>
CCDK_FORCEINLINE unique(ForwardIt begin, ForwardIt end) {
	auto prev = begin;
	for (auto it = ++begin; it != end; ++it) {
		if()
	}
}


ccdk_namespace_mpl_it_end