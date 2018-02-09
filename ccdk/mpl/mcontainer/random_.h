#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/mcontainer/mcontainer_fwd.h>
#include<ccdk/mpl/mcontainer/entirety_.h>

ccdk_namespace_mpl_start

/* at : get type at Index of Container */
template<typename Container, int32 Index>
struct at_ :Container::template at<Index> {};

template<typename Container, int32 Index>
using at_t = typename at_<Container, Index>::type;

/* insert at */
template<typename Container, int32 Index, typename T>
struct insert_at_ : Container::template insert_at<Index, T>{};
template<typename Container, int32 Index, typename T>
using insert_at_t = typename insert_at_<Container, Index, T>::type;

/* erase at */
template<typename Container, int32 Index>
struct erase_at_ : Container::template erase_at<Index>{};
template<typename Container, int32 Index>
using erase_at_t = typename erase_at_<Container, Index>::type;

/* erase [Start, End) */
template<typename Container, int32 Start, int32 End = Start+1, bool =(End==Start+1)>
struct erase_
{
	static_assert(Start >= 0 && End > Start && End <= size_v<Container>, "out of range");
	typedef split_head_t<Container,Start> head;
	typedef split_tail_t<Container, End>  tail;
	typedef merge_t<head, tail>           type;
};

/* erase only 1-elements */
template<typename Container, int32 Start, int32 End>
struct erase_<Container, Start, End, true>: erase_at_<Container,Start> {};

template<typename Container, int32 Start, int32 End = Start+1>
using erase_t = typename erase_<Container, Start, End>::type;

template<typename int32 Start, int32 End, typename... Args>
struct erase_args:erase_<arg_pack<Args...>, Start, End>{};

ccdk_namespace_mpl_end