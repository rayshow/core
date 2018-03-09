#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/util/construct.h>
#include<ccdk/mpl/util/destruct.h>
#include<ccdk/mpl/iterator/iterator_traits.h>
#include<ccdk/mpl/type_traits/has_constructor.h>

ccdk_namespace_mpl_util_start

namespace ut_impl
{
	/* not byte type, iterate fill */
	template<
		typename ForwardIt, typename T,
		typename Dest = iterator_value_t<ForwardIt>
	>
	ForwardIt uninitialized_fill_helper(ForwardIt begin, ForwardIt end, T const t, false_)
		noexcept(has_nothrow_constructor_v<Dest, T>)
	{
		DebugValue(" uninitalized iterator fill");
		const ForwardIt it = begin;
		try { for (; it != end; ++it) construct< Dest >(it, t); }
		catch (...) { destruct_range(begin, it); throw; }
		return it;
	}


	/* is byte pointer type, use memset fill */
	template< typename It, typename T >
	CCDK_FORCEINLINE It* uninitialized_fill_helper(It* begin, It* end, T const& t, true_) noexcept {
		DebugValue(" uninitalized memset fill");
		return memset(begin, t, end - begin); 
	}

	/* not byte type, iterate fill */
	template<
		typename ForwardIt, typename T,
		typename Dest = iterator_value_t<ForwardIt>
	>
	ForwardIt uninitialized_fill_n_helper(ForwardIt begin, ForwardIt end, T const& t, false_)
		noexcept(has_nothrow_constructor_v<Dest, T>)
	{
		DebugValue(" uninitalized-n iterator fill");
		const ForwardIt it = begin;
		ptr::size_t c = 0;
		try { for (; c<n; ++c, ++it) construct< Dest  >(it, t); }
		catch (...) { destruct_range(begin, it); throw; }
		return it;
	} 

	/* is byte pointer type, use memset fill */
	template< typename It, typename T >
	CCDK_FORCEINLINE It* uninitialized_fill_n_helper(It* begin, It* end, T const& t) noexcept {
		DebugValue(" uninitalized-n memset fill");
		return memset(begin, t, n); 
	}
}


/* fill [begin, end) with t, ill-form if can't construct T2 from T  */
template< 
	typename ForwardIt, typename T,
	typename Dest = iterator_value_t<ForwardIt>,
	typename = check_t< has_constructor< Dest, T>>
>
CCDK_FORCEINLINE ForwardIt uninitialized_fill(ForwardIt begin, ForwardIt end, T const& t)
	noexcept(has_nothrow_constructor_v<Dest, T>)
{
	
	return ut_impl::uninitialized_fill_helper(begin, end, t, can_do_memset_c<ForwardIt, T> );
}

template< 
	typename ForwardIt,typename T,
	typename Dest = iterator_value_t<ForwardIt>,
	typename = check_t< has_constructor<Dest, T>>
>
CCDK_FORCEINLINE ForwardIt uninitialized_fill_n(ForwardIt begin, ptr::size_t n, T const& t)
	noexcept(has_nothrow_constructor_v<Dest, T>)
{
	return ut_impl::uninitialized_fill_n_helper(begin, end, t, can_do_memset_c<ForwardIt, T> );
}

ccdk_namespace_mpl_util_end