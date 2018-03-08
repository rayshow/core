#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/base/logic_.h>
#include<ccdk/mpl/type_traits/is_pointer.h>
#include<ccdk/mpl/type_traits/is_byte.h>
#include<ccdk/mpl/type_traits/is_trivial.h>
#include<ccdk/mpl/util/construct.h>
#include<ccdk/mpl/util/destruct.h>
#include<ccdk/mpl/util/addressof.h>
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
		const InputIt it = begin;
		const ForwardIt it2 = begin2;
		try { for (; it != end; ++it, ++it2) construct< Dest >( addressof(*it2), *it); }
		catch (...) { destruct_range(begin2, it2); throw; }
		return it2;
	}

	/* is byte pointer type, use memset fill */
	template< typename T1, typename T2 >
	CCDK_FORCEINLINE T1* uninitialized_copy_helper(T1* begin, T1* end, T2* begin2, true_) { return memcpy(begin2, begin, end - begin); }

	/* not byte type, iterate fill */
	template<
		typename InputIt, typename ForwardIt,
		typename Source = iterator_value_t<InputIt>,
		typename Dest = iterator_value_t<ForwardIt>
	>
		ForwardIt uninitialized_copy_n_helper(InputIt begin, ptr::size_t n, ForwardIt begin2, false_)
		noexcept(has_nothrow_constructor_v<Dest, Source>)
	{
		const InputIt = begin;
		const ForwardIt it2 = begin2;
		try { for (ptr::size_t c = 0; c<n; ++c, ++it, ++it2) construct< Dest  >(addressof(*it), *it2); }
		catch (...) { destruct_range(begin2, it2); throw; }
		return it2;
	}

	/* is byte pointer type, use memset fill */
	template< typename T1, typename T2 >
	CCDK_FORCEINLINE T1* uninitialized_copy_n_helper(T1* begin, ptr::size_t n, T2 begin2) { return memcpy(begin2, begin, n); }
}

template<typename It1, typename It2, typename T1, typename T2>
struct can_do_memcpy : and_< is_pointer<It1>, is_pointer<It2>,
			is_trivial<T1>, is_trivial<T2>,
			has_trivial_constructor<T1,T2>,
			bool_<sizeof(T1)==sizeof(T2)>  > {};

/* fill [begin, end) with t */
template<
	typename InputIt, typename ForwardIt,
	typename Source = iterator_value_t<InputIt>,
	typename Dest = iterator_value_t<ForwardIt>,
>
CCDK_FORCEINLINE ForwardIt uninitialized_copy(InputIt begin, InputIt end, ForwardIt begin2)
{
	return ut_impl::uninitialized_copy_helper(begin, end, begin2, typename can_do_memcpy<Source, Dest>::type{});
}

/* copy [begin, begin+n) to [begin2, begin2 + n) */
template<
	typename InputIt, typename ForwardIt,
	typename Source = iterator_value_t<InputIt>,
	typename Dest = iterator_value_t<ForwardIt>,
>
CCDK_FORCEINLINE ForwardIt uninitialized_copy_n(InputIt begin, ptr::size_t n, ForwardIt begin2)
noexcept(has_nothrow_constructor_v<Dest, Source>)
{
	return ut_impl::uninitialized_copy_n_helper(begin, end, begin2, typename can_do_memcpy<Source, Dest>::type{});
}

ccdk_namespace_mpl_util_end