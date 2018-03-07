#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/util/construct.h>
#include<ccdk/mpl/util/addressof.h>
#include<ccdk/mpl/iterator/iterator_traits.h>
#include<ccdk/mpl/type_traits/has_constructor.h>

ccdk_namespace_mpl_it_start

template<
	typename InputIt,
	typename ForwardIt,
	typename Target = iterator_value_t<ForwardIt> ,
	typename Source = iterator_value_t<InputIt>
>
constexpr void uninitialized_copy(InputIt begin, InputIt end, ForwardIt begin2)
	noexcept(has_nothrow_constructor_v<Target, Source> )
{
	const ForwardIt it2 = begin2;
	const InputIt it = begin;
	try 
	{
		for (; it != end; ++begin, ++it2)
		{
			construct< Target  >(addressof(*it2), *it);
		}
	}
	catch (...)
	{
		for (; begin2 != it2; ++begin2)
		{
			destruct< iterator_value_t<ForwardIt> >(addressof(*begin2));
		}
		throw;
	}
}


template<typename T1, typename T2>
void uninitialized_copy(T1 const* begin, T1 const* end, T2 * begin2) 
		noexcept(has_nothrow_constructor_v<T2,T1>)
{
	T2 const* it2 = begin2;
	try
	{
		for (T1 const* it = begin; it < end; = ++it, ++it2)
		{
			construct< T1 >(it2, *it);
		}
	}
	catch (...)
	{
		for (; begin2 != it2; ++begin2)
		{
			destruct< T2 >(begin2);
		}
		throw;
	}
}



template<
	typename InputIt,
	typename ForwardIt,
	typename Target = iterator_value_t<ForwardIt>,
	typename Source = iterator_value_t<InputIt>
>
void uninitialized_copy_n(InputIt begin1, ptr::size_t count, ForwardIt begin2)
{
	const ForwardIt it2 = begin2;
	ptr::size_t c = 0;
	try
	{
		for (;c<count; ++c, ++begin1, ++it2)
		{
			construct< Target  >(addressof(*it2), *begin1);
		}
	}
	catch (...)
	{
		for (; begin2 != it2; ++begin2)
		{
			destruct< Target >(addressof(*begin2));
		}
		throw;
	}
}


template<typename T1, typename T2>
void uninitialized_copy_n(T1* begin1, ptr::size_t count, T2 * begin2)
		noexcept(has_nothrow_constructor_v<T2, T1>)
{
	T2 const* it2 = begin2;
	try
	{
		for (T1 const* it = begin; it < end; = ++it, ++it2)
		{
			construct< T1 >(it2, *it);
		}
	}
	catch (...)
	{
		for (; begin2 != it2; ++begin2)
		{
			destruct< T2 >(begin2);
		}
		throw;
	}
}



ccdk_namespace_mpl_it_end