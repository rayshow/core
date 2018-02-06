#pragma once

#include<ccdk/mpl/base/apply_.h>
#include<ccdk/mpl/base/derive_if.h>
#include<ccdk/mpl/base/identity.h>
#include<ccdk/mpl/mcontainer/iterator_.h>
#include<ccdk/mpl/mcontainer/extensible_.h>
#include<ccdk/mpl/type_traits/is_same.h>



ccdk_namespace_mpl_start


template<typename Begin, typename End, CCDK_MPL_TFN1(Pred)>
struct iter_find_if_ : derive_if< 
						Pred<deref_t<Begin>>,
						identity<Begin>,
						iter_find_if_<next_t<Begin>, End, Pred> > {};

template<typename End, CCDK_MPL_TFN1(Pred)>
struct iter_find_if_<End, End, Pred> { typedef End type; };

template<typename Begin, typename End, CCDK_MPL_TFN1(Pred)>
using iter_find_if_t = typename iter_find_if_<Begin, End, Pred>::type;

template<typename Begin, typename End, typename T>
struct iter_find_:iter_find_if_< Begin, End, 

ccdk_namespace_mpl_end