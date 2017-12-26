#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/base/and_.h>
#include<ccdk/mpl/base/or_.h>
#include<ccdk/mpl/type_traits/is_convertible.h>
#include<ccdk/mpl/type_traits/is_compatible.h>
#include<ccdk/mpl/type_traits/is_same.h>
#include<ccdk/mpl/type_traits/is_array.h>
#include<ccdk/mpl/type_traits/remove_dim.h>

ccdk_namespace_mpl_sp_start

template< typename T, typename D, typename R>
class share_ptr_base;

template< typename T,typename D, typename R>
class share_ptr;

template<typename T, typename D, typename R>
class weak_ptr;

template< typename T, typename R>
class poly_share_ptr;

template<typename T, typename R>
class poly_weak_ptr;

template< typename T, typename D>
class unique_ptr_base;


ccdk_namespace_mpl_sp_end


ccdk_namespace_mpl_start

#define ccdk_sp_t ccdk::mpl::sp::share_ptr_base
#define ccdk_up_t ccdk::mpl::sp::unique_ptr_base
#define ccdk_psp_t ccdk::mpl::sp::poly_share_ptr

// share_ptr S1 convertible to S2 when
//   0. both is share_ptr
//   1. same element array or single type T1* convertible to T2*
//   2. same delete_type
//   3. same ref_count_type
template<typename T1, typename T2, typename D1, typename D2, typename R1, typename R2>
struct is_convertible< ccdk_sp_t< T1, D1, R1>, ccdk_sp_t< T2, D2, R2>>
	: and_< or_< is_convertible<T1*, T2*>,
				 and_< is_array<T1>, is_array<T2>, is_same< remove_dim_t<T1>, remove_dim_t<T2> >> >,
			is_same<D1, D2>,
			is_same<R1, R2>
	      > {};

template<typename T1, typename T2, typename D1, typename D2, typename R1, typename R2>
struct is_compatible< ccdk_sp_t< T1, D1, R1>, ccdk_sp_t< T2, D2, R2>>
	: and_< or_< is_compatible<T1*, T2*>,
			and_< is_array<T1>, is_array<T2>, is_same< remove_dim_t<T1>, remove_dim_t<T2> >>>,
	is_same<D1, D2>,
	is_same<R1, R2>
	> {};

//poly sp to poly sp
//  1. T1* is comvertible to T2* or T1 T2 is same type array
//  2. same ref_count type    
template<typename T1, typename T2, typename R1, typename R2>
struct is_convertible< ccdk_psp_t<T1,R1>, ccdk_psp_t<T2,R2> > : and_< is_convertible<T1*, T2*>, is_same<R1, R2>>  {};

//poly share pointer compatible
template<typename T1, typename T2, typename R1, typename R2>
struct is_compatible< ccdk_psp_t<T1, R1>, ccdk_psp_t<T2, R2> > : and_< is_compatible<T1*, T2*>, is_same<R1, R2>> {};


//unique_ptr 
template<typename T1, typename T2, typename R1, typename R2>
struct is_convertible< ccdk_up_t<T1, R1>, ccdk_up_t<T2, R2> > : and_< is_convertible<T1*, T2*>, is_same<R1, R2>> {};

//poly share pointer compatible
template<typename T1, typename T2, typename R1, typename R2>
struct is_compatible< ccdk_up_t<T1, R1>, ccdk_up_t<T2, R2> > : and_< is_compatible<T1*, T2*>, is_same<R1, R2>> {};


#undef ccdk_sp_t
#undef ccdk_wp_t
#undef ccdk_psp_t
#undef ccdk_pwp_t

ccdk_namespace_mpl_end