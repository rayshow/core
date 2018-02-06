#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/base/type_.h>
#include<ccdk/mpl/base/apply_.h>
#include<ccdk/mpl/base/derive_if.h>
#include<ccdk/mpl/base/apply_.h>
#include<ccdk/mpl/type_traits/is_same.h>
#include<ccdk/mpl/mcontainer/mcontainer_fwd.h>
#include<ccdk/mpl/mcontainer/iterator_.h>

ccdk_namespace_mpl_start

/* get index if Pred is true */
template<typename Begin, typename End, typename Pred, int32 Index=0>
struct iter_index_if_ : 
		derive_if< apply_<Pred, deref_t<Begin>>, int32_<Index>, 
			iter_index_if_< next_t<Begin>, End, Pred, Index + 1>>
{};

/* not found is -1 */
template<typename End, typename Pred, int32 Index>
struct iter_index_if_<End, End,Pred,Index>  :int32_< -1> {};

/* get index of T */
template<typename Begin, typename End, typename T>
struct iter_index_ : iter_index_if_<Begin, End, add_apply_<is_same, T>> {};

/* get index from container if Pred is true */
template< typename Container, typename Pred>
struct index_if_ :iter_index_if_<begin_t<Container>, end_t<Container>, Pred> {};

/* get index of T from container */
template< typename Container, typename T>
struct index_ :iter_index_< begin_t<Container>, end_t<Container>, T> {};

/* alias */
template<typename Begin, typename End, typename Pred>
static constexpr int32 iter_index_if_v = iter_index_if_<Begin, End, Pred>::value;

template<typename Begin, typename End, typename T>
static constexpr int32 iter_index_v = iter_index_<Begin, End, T>::value;

template<typename Container, typename T>
static constexpr int32 index_v = index_<Container, T>::value;

template<typename Container, typename Pred>
static constexpr int32 index_if_v = index_if_<Container, Pred>::value;

template<typename T, typename... Args>
static constexpr int32 args_index_v = index_< arg_pack<Args...>, T>::value;

ccdk_namespace_mpl_end