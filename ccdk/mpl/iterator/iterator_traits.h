#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/base/logic_.h>
#include<ccdk/mpl/type_traits/is_pointer.h>
#include<ccdk/mpl/type_traits/is_trivial.h>
#include<ccdk/mpl/type_traits/is_byte.h>
#include<ccdk/mpl/type_traits/is_base_of.h>
#include<ccdk/mpl/type_traits/has_assigner.h>
#include<ccdk/mpl/type_traits/has_constructor.h>
#include<ccdk/mpl/type_traits/impl/has_nest_decl.h>
#include<ccdk/mpl/iterator/iterator_fwd.h>

ccdk_namespace_mpl_it_start

// test weather Container::iterator is defined
CCDK_TT_HAS_NEST_TYPE_DECL(iterator, iterator);

// test weather Container::const_iterator is defined
CCDK_TT_HAS_NEST_TYPE_DECL(const_iterator, const_iterator);


template<typename It>
struct iterator_traits
{
	using value_type      = typename It::value_type ;
	using pointer         = typename It::pointer;
	using const_pointer   = typename It::const_pointer;
	using reference       = typename It::reference;
	using const_reference = typename It::const_reference;
	using size_type       = typename It::size_type;
	using difference_type = typename It::difference_type ;
	using category        = typename It::category;
};

template<typename T>
struct iterator_traits<T*>
{
	using value_type      = T;
	using pointer         = T*;
	using const_pointer   = T const*;
	using reference       = T&;
	using const_reference = T const&;
	using difference_type = ptr::diff_t;
	using size_type       = ptr::size_t;
	using category        = random_category;
};

template<typename T>
struct iterator_traits<T const*>
{
	using value_type      = T;
	using pointer         = T const*;
	using const_pointer   = T const*;
	using reference       = T const& ;
	using const_reference = T const&;
	using difference_type = ptr::diff_t;
	using size_type       = ptr::size_t;
	using category        = random_category;
};

/* get value_type from iterator  */
template<typename T>
using iterator_value_t = typename iterator_traits<T>::value_type;

template<typename T>
using iterator_category_t = typename iterator_traits<T>::category;

template<typename T>
constexpr iterator_category_t<T> iterator_category_c{};

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
struct fill_opt_level 
		: derive_if< 
			and_<is_trivial<T1>, has_assigner<T1, ValueType> >,
				derive_if< can_do_memset<It1, ValueType, T1>, opt_lv3, opt_lv2>,
				opt_lv1> {};



/* can do memcpy from T1 to T2 or T2 to T1 */
template< typename It1, typename It2, typename T1,typename T2>
struct can_do_memcpy 
	: and_< is_pointer<It1>, is_pointer<It2>, is_trivial<T1>,
			is_trivial<T2>, bool_<sizeof(T1) == sizeof(T2)>  > {};

//copy opt level trait
template<
	typename It1, typename It2,
	typename T1 = iterator_value_t<It1>,
	typename T2 = iterator_value_t<It2>>
struct copy_opt_level 
	: derive_if<
		and_<is_trivial<T1>, is_trivial<T2>, has_assigner<T1, T2> >,
			derive_if< can_do_memcpy<It1,It2, T1,T2>, opt_lv3, opt_lv2>,
			opt_lv1> {};

template<typename It, typename ValueType>
constexpr typename fill_opt_level<It, ValueType>::type fill_opt_level_c{};

template< typename It1, typename It2>
constexpr typename copy_opt_level<It1, It2>::type copy_opt_level_c{};


template< typename T>
struct is_derived_input_iterator
		:is_base_of< input_category, iterator_category_t<T> > {};

// T is pointer type or T::category exists and category is derive from input_iterator_category 
template<typename T>
struct is_iterator 
		: or_< is_pointer<T>, and_< has_nest_category<T>, 
			   is_derived_input_iterator<T> >> {};

// ptr iterator 
template<
	typename T, 
	typename It, 
	typename P = remove_cv_t< remove_pointer_t<It>>  >
struct is_pointer_iterator 
		: and_< is_same<P,T>, or_ < is_same< It, P*>, is_same<It, P const*> >>{};

ccdk_namespace_mpl_it_end