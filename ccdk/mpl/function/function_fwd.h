#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/base/and_.h>
#include<ccdk/mpl/base/or_.h>
#include<ccdk/mpl/type_traits/is_convertible.h>
#include<ccdk/mpl/type_traits/is_compatible.h>
#include<ccdk/mpl/type_traits/is_same.h>
#include<ccdk/mpl/type_traits/is_array.h>
#include<ccdk/mpl/type_traits/remove_dim.h>
#include<ccdk/mpl/type_traits/has_inner_type.h>

ccdk_namespace_mpl_fn_start

/* function declare */
template<typename T>
struct function;

/* mark value */
template<typename T>
struct value_t {};

/* mark reference */
template<typename T>
struct reference_t {};

/* lazy expr declare */
template<typename... Args>
struct expr;

/* justify is lazy expr */
template<typename T> struct is_expr :false_ {};
template<typename... Args> struct is_expr< expr<Args...> > :true_ {};

/* mfunction judge */
struct mfunction_tag {};

namespace detail
{
	template<typename T>
	struct inner_tag_is_mfunction : is_same<typename T::tag, mfunction_tag> {};
}

template<typename T>
struct is_mfunction : and_< has_inner_tag<T>, detail::inner_tag_is_mfunction<T>> {};




ccdk_namespace_mpl_fn_end


ccdk_namespace_mpl_start

#define ccdk_fn  ::ccdk::mpl::fn::function

/*  function<T1> can transform to  function<T2>  when Ret and each Arg can transform */
template<typename Ret1, typename Ret2, typename... Args1, typename... Args2>
struct is_convertible< ccdk_fn<Ret1(Args1...)>, ccdk_fn<Ret2(Args2...)> > : and_<  is_convertible< Ret1, Ret2>, is_convertible<Args1, Args2>... > {};

/*  function<T1> compatible with function<T2> when Ret and each Arg is compatible */
template<typename Ret1, typename Ret2, typename... Args1, typename... Args2>
struct is_compatible< ccdk_fn<Ret1(Args1...)>, ccdk_fn<Ret2(Args2...)> > : and_<  is_compatible< Ret1, Ret2>, is_compatible<Args1, Args2>... > {};

#undef ccdk_fn

ccdk_namespace_mpl_end