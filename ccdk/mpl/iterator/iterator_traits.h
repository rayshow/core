#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/iterator/iterator_fwd.h>

#include<ccdk/mpl/base/logic_.h>
#include<ccdk/mpl/type_traits/is_pointer.h>
#include<ccdk/mpl/type_traits/is_trivial.h>
#include<ccdk/mpl/type_traits/is_byte.h>
#include<ccdk/mpl/type_traits/is_base_of.h>
#include<ccdk/mpl/type_traits/has_assigner.h>
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
	typedef random_iterator_category category;
};

template<typename T>
struct iterator_traits<T const*>
{
	typedef T           value_type;
	typedef T *          pointer_type;
	typedef T&          reference_type;
	typedef T const&    const_reference_type;
	typedef ptr::diff_t difference_type;
	typedef ptr::size_t size_type;
	typedef random_iterator_category category;
};

/* get value_type from iterator  */
template<typename T>
using iterator_value_t = typename iterator_traits<T>::value_type;

template<typename T>
using iterator_categroy_t = typename iterator_traits<T>::category;

template<typename T>
constexpr iterator_categroy_t<T> iterator_categroy_c{};

struct opt_lv1 { typedef opt_lv1 type; };
struct opt_lv2 :opt_lv1 { typedef opt_lv2 type; };
struct opt_lv3 :opt_lv2 { typedef opt_lv3 type; };

/* fill can do memset to It1 range */
template< typename It1, typename ValueType, typename T1>
struct can_do_memset : and_< is_pointer<It1>, is_byte< T1 > > {};

/* fill optim level */
template<
	typename It1, typename ValueType,
	typename T1 = iterator_value_t<It1>>
	struct fill_opt_level :
	derive_if< 
			and_<is_trivial<T1>, has_assigner<T1, ValueType> >,
			derive_if< can_do_memset<It1, ValueType, T1>, opt_lv3, opt_lv2>,
			opt_lv1> {};



/* can do memcpy from T1 to T2 or T2 to T1 */
template< typename It1, typename It2, typename T1,typename T2>
struct can_do_memcpy : and_< is_pointer<It1>, is_pointer<It2>,
	is_trivial<T1>, is_trivial<T2>,
	bool_<sizeof(T1) == sizeof(T2)>  > {};

/* copy */
template<
	typename It1, typename It2,
	typename T1 = iterator_value_t<It1>,
	typename T2 = iterator_value_t<It2>>
	struct copy_opt_level :
	derive_if<
		and_<is_trivial<T1>, is_trivial<T2>, has_assigner<T1, T2> >,
		derive_if< can_do_memcpy<It1,It2, T1,T2>, opt_lv3, opt_lv2>,
		opt_lv1> {};

template<typename It, typename ValueType>
constexpr typename fill_opt_level<It, ValueType>::type fill_opt_level_c{};

template< typename It1, typename It2>
constexpr typename copy_opt_level<It1, It2>::type copy_opt_level_c{};

/* T is pointer type or T::category exists and category is derive from input_iterator_category */
template<typename T>
struct is_iterator : or_< is_pointer<T>,
		and_< has_inner_categroy<T>, is_base_of< input_iterator_category, T> >> {};


ccdk_namespace_mpl_it_end