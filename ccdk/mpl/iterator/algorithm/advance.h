#pragma once

#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/iterator/iterator_traits.h>

ccdk_namespace_mpl_it_start

namespace alg_impl
{
	template<typename InputIt>
	InputIt advance_impl(InputIt begin, ptr::diff_t n , input_category) noexcept{
		ccdk_assert(n > 0);
		if (n > 0) for (ptr::diff_t i = 0; i < n; ++i, ++begin);
		return begin;
	}

	template<typename BiwardIt>
	BiwardIt advance_impl(BiwardIt begin, ptr::diff_t n , biward_category) noexcept {
		if (n > 0) for (ptr::diff_t i = 0; i < n;++i, ++begin);
		else if (n < 0) for (ptr::diff_t i = n; i < 0; ++i, --begin);
		return begin;
	}

	template<typename RandomIt>
	CCDK_FORCEINLINE RandomIt advance_impl(RandomIt begin, ptr::diff_t n, random_category) noexcept{
		return begin+n;
	}
}

template<typename It, typename = check_t< is_iterator<It>>>
CCDK_FORCEINLINE It advance(It begin, ptr::diff_t n) noexcept
{
	return alg_impl::advance_impl(begin, end, iterator_category_c<It>);
}

ccdk_namespace_mpl_it_end