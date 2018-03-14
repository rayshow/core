#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/type_traits/has_destructor.h>
#include<ccdk/mpl/iterator/iterator_traits.h>

ccdk_namespace_mpl_util_start


/* trivially destructor */
template<typename T>
CCDK_FORCEINLINE static void destruct(void* memory) noexcept
{
	reinterpret_cast<T*>((void*)memory)->~T();
}


namespace ut_impl
{
	/* has trivial destructor, no no-thing */
	template<typename InputIt>
	CCDK_FORCEINLINE static void destruct_range_impl(InputIt begin, InputIt end, true_) noexcept {}

	/* non-pod */
	template<typename InputIt, typename Source = iterator_value_t<InputIt> >
	CCDK_FORCEINLINE static void destruct_range_impl(InputIt begin, InputIt end, false_) noexcept
	{
		for (; begin != end; ++begin) { (*begin).~Source(); }
	}

	/* is pod, no need destructor */
	template<typename InputIt>
	CCDK_FORCEINLINE static void destruct_n_impl(InputIt begin, ptr::size_t n, true_) noexcept {
		DebugValue("trivial destruct-n");
	}

	/* non-pod */
	template<typename InputIt, typename Source = iterator_value_t<InputIt> >
	CCDK_FORCEINLINE static void destruct_n_impl(InputIt begin, ptr::size_t n, false_) noexcept
	{
		DebugValue("iterator destruct-n");
		for (ptr::size_t c = 0; c<n; ++c,++begin) { (*begin).~Source(); }
	}
}

/* destruct [begin, end) */
template<typename InputIt, typename Source = iterator_value_t<InputIt> >
CCDK_FORCEINLINE static void destruct_range(InputIt begin, InputIt end) noexcept
{
	ut_impl::destruct_range_impl(begin, end, typename has_trivial_destructor<Source>::type{} );
}

/* destruct [begin, end) */
template<typename InputIt, typename Source = iterator_value_t<InputIt> >
CCDK_FORCEINLINE static void destruct_n(InputIt begin, ptr::size_t n) noexcept
{
	ut_impl::destruct_n_impl(begin, n, typename has_trivial_destructor<Source>::type{});
}

ccdk_namespace_mpl_util_end