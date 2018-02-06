#pragma once

#include<ccdk/mpl/base/apply_.h>
#include<ccdk/mpl/base/derive_if.h>
#include<ccdk/mpl/base/identity.h>
#include<ccdk/mpl/mcontainer/iterator_.h>
#include<ccdk/mpl/mcontainer/extensible_.h>
#include<ccdk/mpl/type_traits/is_same.h>

ccdk_namespace_mpl_start

/* iterator find */
template<typename Begin, typename End, typename MFn>
struct iter_find_if_
{
	typedef typename derive_if<
		apply_< MFn, deref_t<Begin>>,
		identity< Begin>,
		iter_find_if_< next_t<Begin>, End, MFn>>::type type;
};

template<typename End, typename MFn>
struct iter_find_if_<End,End,MFn> { typedef End type; };

template<typename Begin, typename End, typename T>
struct iter_find_ : iter_find_if_<Begin, End, add_apply_< is_same, T> > {};

/* container find */
template<typename Container, typename MFn>
struct find_if_ :iter_find_if_< begin_t<Container>, end_t<Container>, MFn> {};

template<typename Container, typename T>
struct find_ :iter_find_< begin_t<Container>, next_t<Container>, T> {};

/* alias */
template<typename Begin, typename End, typename MFn>
using iter_find_if_t = typename iter_find_if_<Begin, End, MFn>::type;

template<typename Begin, typename End, typename T>
using iter_find_t = typename iter_find_<Begin, End, T>::type;

template<typename Container, typename MFn>
using find_if_t = typename find_if_<Container, MFn>::type;

template<typename Container, typename T>
using find_t = typename find_<Container, T>::type;

ccdk_namespace_mpl_end