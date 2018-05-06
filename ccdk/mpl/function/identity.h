#pragma once

#include<ccdk/mpl/mpl_module.h>

ccdk_namespace_mpl_fn_start

struct identity_t {
	template<typename T>
	CCDK_FORCEINLINE constexpr decltype(auto)
		operator()(T const& t) const noexcept {
		return t;
	}
};

constexpr identity_t identity{};

ccdk_namespace_mpl_fn_end