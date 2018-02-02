#pragma once

#include<ccdk/mpl/mpl_module.h>

ccdk_namespace_mpl_start

/* if true, inner type is T1, otherwise is T2 */
template<bool B, typename T1, typename T2> struct if_c  { typedef T1 type; };
template<typename T1, typename T2> struct if_c<false, T1, T2>  { typedef T2 type; };

/* if true, inner type is T1, otherwise is T2 */
template<bool B, typename T1, typename T2> struct eval_if_c { typedef typename T1::type type; };
template<typename T1, typename T2> struct eval_if_c<false, T1, T2> { typedef typename T2::type type; };

/* wrap version */
template<typename C, typename T1, typename T2> struct if_ :public if_c<C::value, T1, T2> {};

/* wrap version */
template<typename C, typename T1, typename T2> struct eval_if_ :public eval_if_c<C::value, T1, T2> {};

template<bool B, typename T1, typename T2> using if_ct = typename if_c<B, T1, T2>::type;
template<typename C, typename T1, typename T2> using if_t = typename if_<C, T1, T2>::type;

ccdk_namespace_mpl_end