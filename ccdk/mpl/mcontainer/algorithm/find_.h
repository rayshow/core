#pragma once

#include<ccdk/mpl/base/apply_.h>
#include<ccdk/mpl/base/derive_if.h>
#include<ccdk/mpl/base/identity.h>
#include<ccdk/mpl/type_traits/is_same.h>
#include<ccdk/mpl/mcontainer/iterator_.h>
#include<ccdk/mpl/mcontainer/entirety_.h>
#include<ccdk/mpl/mcontainer/algorithm/lambda_.h>

ccdk_namespace_mpl_start

namespace mpl_impl
{
	template<typename Begin, typename End, typename Pred>
	struct iter_find_if_impl:
		derive_if<
			apply_< Pred, deref_t<Begin>>,
				identity< Begin>,
				iter_find_if_impl< next_t<Begin>, End, Pred>> {};

	template<typename End, typename MFn>
	struct iter_find_if_impl<End, End, MFn> { typedef End type; };
}

/* iterator find */
template<typename Begin, typename End, typename MFn>
struct iter_find_if_ : mpl_impl::iter_find_if_impl<Begin, End, lambda_<MFn>> {};

template<typename Begin, typename End, typename T>
struct iter_find_ : mpl_impl::iter_find_if_impl<Begin, End, add_apply_< is_same, T> > {};

/* container find */
template<typename Container, typename MFn>
struct find_if_ :iter_find_if_< begin_t<Container>, end_t<Container>, MFn> {};

template<typename Container, typename T>
struct find_ :iter_find_< begin_t<Container>, end_t<Container>, T> {};

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