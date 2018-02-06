#pragma once

#include<ccdk/mpl/base/apply_.h>
#include<ccdk/mpl/base/derive_if.h>
#include<ccdk/mpl/type_traits/is_same.h>
#include<ccdk/mpl/mcontainer/iterator_.h>
#include<ccdk/mpl/mcontainer/extensible_.h>

ccdk_namespace_mpl_start

/* for iterator count */
template< typename Begin, typename End, typename MFn, typename Count = uint32_<0> >
struct iter_count_if_:
			iter_count_if_< next_t<Begin>, End, MFn,
				derive_if< apply_<MFn, deref_t<Begin>>, next_t<Count>, Count>
			> 
{};

template< typename End, typename MFn, typename Count>
struct iter_count_if_<End, End, MFn, Count>:Count {};

template< typename Begin, typename End, typename T>
struct iter_count_ : iter_count_if_< Begin, End, add_apply_<is_same, T> > {};

/* for container count */
template< typename Container, typename MFn>
struct count_if_: iter_count_if_< begin_t<Container>,  end_t<Container>, MFn> { };

template< typename Container, typename T>
struct count_ : iter_count_< begin_t<Container>, end_t<Container>, T> { };


/* alias */
template< typename Begin, typename End,  typename MFn>
static constexpr uint32 iter_count_if_v = iter_count_if_<Begin, End, MFn>::value;

template< typename Container, typename MFn>
static constexpr uint32 count_if_v = count_if_<Container, MFn>::value;

template< typename Begin, typename End, typename T>
static constexpr uint32 iter_count_v = iter_count_<Begin, End, T>::value;

template< typename Container, typename T>
static constexpr uint32 count_v = count_<Container, T>::value;

ccdk_namespace_mpl_end