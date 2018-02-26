#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/iterator/iterator_fwd.h>
ccdk_namespace_mpl_it_start


template<typename It>
struct iterator_traits
{
	typedef typename It::value_type value_type;
	typedef value_type*             pointer_type;
	typedef value_type&             reference_type;
	typedef value_type const&       const_reference_type;
	typedef typename It::size_type  size_type;
	typedef typename It::difference_type difference_type;
	typedef typename It::category   category;
};

template<typename T>
struct iterator_traits<T*>
{
	typedef T           value_type;
	typedef T*          pointer_type;
	typedef T&          reference_type;
	typedef T const&    const_reference_type;
	typedef ptr::diff_t difference_type;
	typedef ptr::size_t size_type;
	typedef random_iterator_category categroy;
};

template<typename T>
using iterator_value_t = typename iterator_traits<T>::value_type;



ccdk_namespace_mpl_it_end