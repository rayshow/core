#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/type_traits/has_equal.h>

ccdk_namespace_mpl_fn_start

struct not_null_t {

	template<
		typename T, 
		typename = check_t< has_equal<T,ptr::nullptr_t> >
	>
	CCDK_FORCEINLINE constexpr bool operator()(T const& t) const noexcept {
		return !(t == nullptr);
	}
};

constexpr not_null_t not_null{};


ccdk_namespace_mpl_fn_end