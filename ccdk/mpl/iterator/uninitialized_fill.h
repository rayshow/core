#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/util/construct.h>
#include<ccdk/mpl/util/destruct.h>
#include<ccdk/mpl/util/addressof.h>
#include<ccdk/mpl/iterator/iterator_traits.h>
#include<ccdk/mpl/type_traits/has_constructor.h>

ccdk_namespace_mpl_it_start

template<
	typename ForwardIt,
	typename T,
	typename Source = iterator_value_t<ForwardIt>
>
constexpr void uninitialized_fill(ForwardIt begin, ForwardIt end, T const& t)
noexcept(has_nothrow_constructor_v<Source, T>)
{
	const ForwardIt it = begin;
	try
	{
		for (; it != end; ++it)
		{
			construct< Source >(addressof(*it), t);
		}
	}
	catch (...)
	{
		for (; begin != it; ++begin)
		{
			destruct< Source >(addressof(*begin));
		}
		throw;
	}
}

template<
	typename ForwardIt,
	typename T2,
	typename Target = iterator_value_t<ForwardIt>
>
void uninitialized_fill_n(ForwardIt begin, ptr::size_t n, T2 const& t2)
{
	const ForwardIt it = begin;
	ptr::size_t c = 0;
	try
	{
		for (; c<n; ++c, ++it)
		{
			construct< Target  >(addressof(*it), t2);
		}
	}
	catch (...)
	{
		for (n=0; n < c; ++n, ++begin)
		{
			destruct< Target >(addressof(*begin));
		}
		throw;
	}
}



ccdk_namespace_mpl_it_end