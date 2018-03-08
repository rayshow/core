#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/type_traits/has_destructor.h>
#include<ccdk/mpl/iterator/iterator_traits.h>

ccdk_namespace_mpl_util_start


/* trivially destructor */
template<typename T>
CCDK_FORCEINLINE static constexpr void destruct(void* memory) 
	noexcept( has_nothrow_destructor_v<T> )
{
	reinterpret_cast<T*>(memory)->~T();
}

namespace ut_impl
{
	/* is pod, no need destructor */
	template<typename ForwardIt>
	CCDK_FORCEINLINE static void destruct_range_helper(ForwardIt begin, ForwardIt end, true_) noexcept {}

	/* non-pod */
	template<typename ForwardIt, typename Source = iterator_value_t<ForwardIt> >
	CCDK_FORCEINLINE static void destruct_range_helper(ForwardIt begin, ForwardIt end, false_)
		noexcept(has_nothrow_destructor_v< Source >)
	{
		for (ForwardIt it = begin; it != end; ++it) { (*it).~Source(); }
	}
}

/* destruct [begin, end) */
template<typename ForwardIt, typename Source = iterator_value_t<ForwardIt> >
CCDK_FORCEINLINE static constexpr void destruct_range(ForwardIt begin, ForwardIt end)
	noexcept(has_nothrow_destructor_v< Source >)
{
	ut_impl::destruct_range_helper(begin, end, typename has_trivial_destructor<Source>::type{} );
}

ccdk_namespace_mpl_util_end