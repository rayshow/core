#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/base/and_.h>
#include<ccdk/mpl/base/or_.h>
#include<ccdk/mpl/type_traits/is_convertible.h>
#include<ccdk/mpl/type_traits/is_same.h>
#include<ccdk/mpl/type_traits/is_array.h>
#include<ccdk/mpl/type_traits/remove_dim.h>


ccdk_namespace_mpl_sp_start

template<
	typename Class,
	typename Deleter,
	typename RefCount
>
class share_ptr;


// trait T is share_ptr type
template<typename T> struct is_share_ptr :false_ {};
template<typename T, typename D, typename R> struct is_share_ptr< share_ptr<T, D, R>> :true_ {};

ccdk_namespace_mpl_sp_end


ccdk_namespace_typetraits_impl_start


// share_ptr S1 convertible to S2 when
//   0. both is share_ptr
//   1. same element array or single type T1* convertible to T2*
//   2. same delete_type
//   3. same ref_count_type
template<typename T1, typename T2, typename D1, typename D2, typename R1, typename R2>
struct is_convertible< ccdk::mpl::sp::share_ptr< T1, D1, R1>, ccdk::mpl::sp::share_ptr< T2, D2, R2>>
	: and_< or_< 
				 is_convertible<T1*, T2*>,
				 and_< is_array<T1>, is_array<T2>, is_same< remove_dim_t<T1>, remove_dim_t<T2> >>
			   >,
			is_same<D1, D2>,
			is_same<R1, R2>
	      > {};

ccdk_namespace_typetraits_impl_end