#pragma once

#include<ccdk/mpl/mpl_module.h>

ccdk_namespace_mpl_start

/*  for non-inner-contained-type type
	if(B1)      type = Case2
	else if(B2) type = Case1
	else        type = default 
*/
template<bool B1, bool B2, typename Case1, typename Case2, typename Default>
struct choose_c { typedef Default type; };

template<typename Case1, typename Case2, typename Default>
struct choose_c<true, false, Case1, Case2, Default> { typedef Case1 type; };

template<typename Case1, typename Case2, typename Default>
struct choose_c<false, true, Case1, Case2, Default> { typedef Case2 type; };


template<typename B1, typename B2, typename Case1, typename Case2, typename Default>
struct choose_ :public choose_c<B1::value, B2::value, Case1, Case2, Default> {};

/*  for non-inner-contained-type type
	if(B1)      derive Case2
	else if(B2) derive Case1
	else        derive default
*/
template<bool B1, bool B2, typename Case1, typename Case2, typename Default>
struct derive_choose_c:public Default {};

template<typename Case1, typename Case2, typename Default>
struct derive_choose_c<true, false, Case1, Case2, Default>:public Case1 {};

template<typename Case1, typename Case2, typename Default>
struct derive_choose_c<false, true, Case1, Case2, Default>:public Case2 {};

template<typename B1, typename B2, typename Case1, typename Case2, typename Default>
struct derive_choose :public derive_choose_c<B1::value, B2::value, Case1, Case2, Default> {};

ccdk_namespace_mpl_end