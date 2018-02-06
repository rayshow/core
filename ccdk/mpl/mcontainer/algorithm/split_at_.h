#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/mcontainer/mcontainer_fwd.h>
#include<ccdk/mpl/mcontainer/forward_.h>
#include<ccdk/mpl/mcontainer/backward_.h>
#include<ccdk/mpl/mcontainer/iterator_.h>
#include<ccdk/mpl/mcontainer/extensible_.h>

ccdk_namespace_mpl_start

namespace mpl_impl
{
	template<typename Head, typename Tail, uint32 Index, uint32 IgnoreCount>
	struct split_at_impl : 
				split_at_impl < 
					push_back_t< Head, front_t< Tail>>, 
					pop_front_t< Tail>, Index - 1, IgnoreCount > {};


	template<typename Head, typename Tail, uint32 IgnoreCount>
	struct split_at_impl<Head, Tail, 0, IgnoreCount>
	{
		typedef Head                             head;
		typedef pop_front_n_t<Tail, IgnoreCount> tail;
		typedef merge_t<head, tail>              type;
	};
}

/* split Container at Index to head and tail, ingore IngoreCount element of tail  */
template<typename Container, uint32 Index, uint32 IgnoreCount = 0>
struct split_at_ : mpl_impl::split_at_impl< clear_t<Container>, Container, Index, IgnoreCount> {};

template<typename Container, uint32 Index, uint32 IgnoreCount = 0>
using split_head_at = typename split_at_<Container, Index, IgnoreCount>::head;
 
template<typename Container, uint32 Index, uint32 IgnoreCount = 0>
using split_tail_at = typename split_at_<Container, Index, IgnoreCount>::tail;

/* split Args... */
template<uint32 Index, uint32 IgnoreCount, typename... Args>
using split_args = split_at_< arg_pack<Args...>, Index, IgnoreCount>;

template<uint32 Index, typename... Args>
using args_head = typename split_args<Index, 0, Args...>::head;

template<uint32 Index , typename... Args>
using args_tail = typename split_args<Index, 0, Args...>::tail;

template<uint32 Index, uint32 IgnoreCount, typename... Args>
using args_erase = typename split_args<Index, IgnoreCount, Args...>::type;

ccdk_namespace_mpl_end