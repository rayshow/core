#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/iterator/iterator_fwd.h>

#include<ccdk/mpl/base/logic_.h>
#include<ccdk/mpl/type_traits/is_pointer.h>
#include<ccdk/mpl/type_traits/is_trivial.h>
#include<ccdk/mpl/type_traits/is_byte.h>
#include<ccdk/mpl/type_traits/is_base_of.h>
#include<ccdk/mpl/type_traits/has_constructor.h>


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

/* get value_type from iterator  */
template<typename T>
using iterator_value_t = typename iterator_traits<T>::value_type;


/* can do memset to It1 range */
template<
	typename It1, 
	typename ValueType,
	typename T1 = iterator_value_t<It1>>
struct can_do_memset : and_< is_pointer<It1>, is_byte< T1 > > {};

/* can do memcpy from T1 to T2 or T2 to T1 */
template<
	typename It1, typename It2,
	typename T1 = iterator_value_t<It1>,
	typename T2 = iterator_value_t<It2>>
struct can_do_memcpy : and_< is_pointer<It1>, is_pointer<It2>,
	is_trivial<T1>, is_trivial<T2>,
	bool_<sizeof(T1) == sizeof(T2)>  > {};

template<typename It, typename ValueType>
constexpr typename can_do_memset<It, ValueType>::type can_do_memset_c{};

template< typename It1, typename It2>
constexpr typename can_do_memcpy<It1, It2>::type can_do_memcpy_c{};

/* T is pointer type or T::category exists and category is derive from input_iterator_category */
template<typename T>
class is_iterator : or_< is_pointer<T>,
		and_< has_inner_categroy<T>, is_base_of< input_iterator_category, T> >> {};


ccdk_namespace_mpl_it_end