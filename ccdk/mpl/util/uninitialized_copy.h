#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/util/construct.h>
#include<ccdk/mpl/util/destruct.h>
#include<ccdk/mpl/iterator/iterator_traits.h>
#include<ccdk/mpl/type_traits/has_constructor.h>

ccdk_namespace_mpl_util_start

namespace ut_impl
{
	/* no-optim */
	template<
		typename InputIt, typename ForwardIt,
		typename Source = iterator_value_t<InputIt>,
		typename Dest = iterator_value_t<ForwardIt>
	>
		ForwardIt uninitialized_copy_helper(InputIt begin, InputIt end, ForwardIt begin2, false_)
			noexcept(has_nothrow_constructor_v<Dest, Source>)
	{
		DebugValue(" uninitalized iterator copy");
		const InputIt it = begin;
		const ForwardIt it2 = begin2;
		try { for (; it != end; ++it, ++it2) construct< Dest >( it2, *it); }
		catch (...) { destruct_range(begin2, it2); throw; }
		return it2;
	}

	/* is byte pointer type, use memset fill */
	template< typename T1, typename T2 >
	CCDK_FORCEINLINE T1* uninitialized_copy_helper(T1* begin, T1* end, T2* begin2, true_) noexcept {
		DebugValue(" uninitialzied memcpy copy");
		return memcpy(begin2, begin, end - begin); 
	}

	/* not byte type, iterate fill */
	template<
		typename InputIt, typename ForwardIt,
		typename Source = iterator_value_t<InputIt>,
		typename Dest = iterator_value_t<ForwardIt>
	>
		ForwardIt uninitialized_copy_n_helper(InputIt begin, ptr::size_t n, ForwardIt begin2, false_)
		noexcept(has_nothrow_constructor_v<Dest, Source>)
	{
		DebugValue(" uninitalized iterator-n copy");
		const InputIt = begin;
		const ForwardIt it2 = begin2;
		try { for (ptr::size_t c = 0; c<n; ++c, ++it, ++it2) construct< Dest  >( it, *it2); }
		catch (...) { destruct_range(begin2, it2); throw; }
		return it2;
	}

	/* is byte pointer type, use memset fill */
	template< typename T1, typename T2 >
	CCDK_FORCEINLINE T1* uninitialized_copy_n_helper(T1* begin, ptr::size_t n, T2 begin2) noexcept {
		DebugValue(" uninitialzied memcpy-n copy");
		return memcpy(begin2, begin, n); 
	}
}

/* fill [begin, end) with t */
template<
	typename InputIt, typename ForwardIt,
	typename Source = iterator_value_t<InputIt>,
	typename Dest = iterator_value_t<ForwardIt>
>
CCDK_FORCEINLINE ForwardIt uninitialized_copy(InputIt begin, InputIt end, ForwardIt begin2)
{
	return ut_impl::uninitialized_copy_helper(begin, end, begin2, can_do_memcpy_c<Source, Dest>);
}

/* copy [begin, begin+n) to [begin2, begin2 + n) */
template<
	typename InputIt, typename ForwardIt,
	typename Source = iterator_value_t<InputIt>,
	typename Dest = iterator_value_t<ForwardIt>
>
CCDK_FORCEINLINE ForwardIt uninitialized_copy_n(InputIt begin, ptr::size_t n, ForwardIt begin2)
noexcept(has_nothrow_constructor_v<Dest, Source>)
{
	return ut_impl::uninitialized_copy_n_helper(begin, end, begin2, can_do_memcpy_c<Source, Dest>);
}

ccdk_namespace_mpl_util_end