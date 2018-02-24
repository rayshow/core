#pragma once

#include<ccdk/mpl/mpl_module.h>

ccdk_namespace_mpl_start
		
/* if true derive T1, else  derive T2 */
template<bool B, typename T1, typename T2> struct derive_if_c:public T1 {};
template<typename T1, typename T2>         struct derive_if_c<false, T1, T2> :public T2 {};

template<bool B, typename T1, typename T2> 
using derive_if_ct = typename derive_if_c<B, T1, T2>::type;

template<typename C, typename T1, typename T2>
struct derive_if :public derive_if_c<C::value, T1, T2> {};

template<typename B, typename T1, typename T2>
using derive_if_t = typename derive_if<B, T1, T2>::type;


ccdk_namespace_mpl_end