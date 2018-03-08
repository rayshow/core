#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/base/logic_.h>
#include<ccdk/mpl/type_traits/is_pointer.h>
#include<ccdk/mpl/type_traits/is_byte.h>
#include<ccdk/mpl/util/construct.h>
#include<ccdk/mpl/util/destruct.h>
#include<ccdk/mpl/util/addressof.h>
#include<ccdk/mpl/iterator/iterator_traits.h>
#include<ccdk/mpl/type_traits/has_constructor.h>

ccdk_namespace_mpl_util_start

namespace ut_impl
{
	/* not byte type, iterate fill */
	template<
		typename ForwardIt, typename T,
		typename Source = iterator_value_t<ForwardIt>
	>
	ForwardIt uninitialized_fill_helper(ForwardIt begin, ForwardIt end, T const t, false_)
		noexcept(has_nothrow_constructor_v<Source, T>) 
	{
		const ForwardIt it = begin;
		try { for (; it != end; ++it) construct< Source >(addressof(*it), t); }
		catch (...) { destruct_range(begin, it); throw; }
		return it;
	}

	/* is byte pointer type, use memset fill */
	template< typename T1, typename T2 >
	CCDK_FORCEINLINE T1* uninitialized_fill_helper(T1* begin, T1* end, T2 value, true_) { return memset(begin, value, end - begin); }

	/* not byte type, iterate fill */
	template<
		typename ForwardIt, typename T,
		typename Source = iterator_value_t<ForwardIt>
	>
	ForwardIt uninitialized_fill_n_helper(ForwardIt begin, ForwardIt end, T const& t, false_)
		noexcept(has_nothrow_constructor_v<Source, T>) 
	{
		const ForwardIt it = begin;
		ptr::size_t c = 0;
		try { for (; c<n; ++c, ++it) construct< Target  >(addressof(*it), t2); }
		catch (...) { destruct_range(begin, it); throw; }
		return it;
	}

	/* is byte pointer type, use memset fill */
	template< typename T1, typename T2 >
	CCDK_FORCEINLINE T1* uninitialized_fill_n_helper(T1* begin, T1* end, T2 value) { return memset(begin, value, n); }
}

template<typename T1, typename T2>
struct can_do_memset : and_< is_pointer<T1>, is_byte< remove_pointer_t<T1>>, is_byte<T2>> {};

/* fill [begin, end) with t */
template<
	typename ForwardIt, typename T,
	typename Source = iterator_value_t<ForwardIt>
>
CCDK_FORCEINLINE ForwardIt uninitialized_fill(ForwardIt begin, ForwardIt end, T const& t)
	noexcept(has_nothrow_constructor_v<Source, T>) 
{
	return ut_impl::uninitialized_fill_helper(begin, end, t, typename can_do_memset<ForwardIt, T>::type{});
}


template< 
	typename ForwardIt,typename T,
	typename Source = iterator_value_t<ForwardIt>
>
CCDK_FORCEINLINE ForwardIt uninitialized_fill_n(ForwardIt begin, ptr::size_t n, T const& t)
	noexcept(has_nothrow_constructor_v<Source, T>) 
{
	return ut_impl::uninitialized_fill_n_helper(begin, end, t, typename can_do_memset<ForwardIt, T>::type{});
}

ccdk_namespace_mpl_util_end