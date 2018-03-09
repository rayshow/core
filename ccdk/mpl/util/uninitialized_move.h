#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/base/logic_.h>
#include<ccdk/mpl/util/construct.h>
#include<ccdk/mpl/util/destruct.h>
#include<ccdk/mpl/iterator/iterator_traits.h>
#include<ccdk/mpl/type_traits/has_constructor.h>
#include<ccdk/mpl/util/move.h>

ccdk_namespace_mpl_util_start

namespace ut_impl
{
	/* no-optim */
	template<
		typename InputIt, typename ForwardIt,
		typename Source = iterator_value_t<InputIt>,
		typename Dest = iterator_value_t<ForwardIt>
	>
	ForwardIt uninitialized_move_helper(InputIt begin, InputIt end, ForwardIt begin2, false_) noexcept {
		DebugValue(" uninitalized iterator move");
		for (; it != end; ++begin, ++begin2) construct< Dest >(begin2, util::move(*begin) );
		return begin2;
	}

	/* is byte pointer type, use memset fill */
	template< typename T1, typename T2 >
	CCDK_FORCEINLINE T1* uninitialized_move_helper(T1* begin, T1* end, T2* begin2, true_) noexcept {
		DebugValue(" uninitalized memcpy move");
		return memcpy(begin2, begin, end - begin); 
	}


	/*  iterate move */
	template<
		typename InputIt, typename ForwardIt,
		typename Source = iterator_value_t<InputIt>,
		typename Dest = iterator_value_t<ForwardIt>
	>
	ForwardIt uninitialized_move_n_helper(InputIt begin, ptr::size_t n, ForwardIt begin2, false_) noexcept {
		DebugValue(" uninitalized-n iterator move");
		for (ptr::size_t c = 0; c<n; ++c, ++begin, ++begin2) construct< Dest  >(begin2, util::move(*begin) );
		return begin2;
	}

	/* can use memset copy */
	template< typename T1, typename T2 >
	CCDK_FORCEINLINE T1* uninitialized_move_n_helper(T1* begin, ptr::size_t n, T2 begin2) noexcept {
		DebugValue(" uninitalized memcpy move");
		return memcpy(begin2, begin, n); 
	}
}

/* fill [begin, end) with t */
template<
	typename InputIt, typename ForwardIt,
	typename Source = iterator_value_t<InputIt>,
	typename Dest = iterator_value_t<ForwardIt>,
>
CCDK_FORCEINLINE ForwardIt uninitialized_move(InputIt begin, InputIt end, ForwardIt begin2) noexcept
{
	return ut_impl::uninitialized_move_helper(begin, end, begin2, can_do_memcpy_c<Source, Dest>);
}

/* copy [begin, begin+n) to [begin2, begin2 + n) */
template<
	typename InputIt, typename ForwardIt,
	typename Source = iterator_value_t<InputIt>,
	typename Dest = iterator_value_t<ForwardIt>,
>
CCDK_FORCEINLINE ForwardIt uninitialized_move_n(InputIt begin, ptr::size_t n, ForwardIt begin2) noexcept
{
	return ut_impl::uninitialized_move_n_helper(begin, end, begin2, can_do_memcpy_c<Source, Dest>);
}

ccdk_namespace_mpl_util_end