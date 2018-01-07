#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/mcontainer/mcontainer_fwd.h>
#include<ccdk/mpl/mcontainer/clear.h>
#include<ccdk/mpl/mcontainer/push_back.h>
#include<ccdk/mpl/mcontainer/pop_front.h>
#include<ccdk/mpl/mcontainer/front.h>
#include<ccdk/mpl/mcontainer/merge.h>

ccdk_namespace_mpl_start

namespace mct_detail
{
	template<typename Head, typename Tail, uint32 Index, uint32 IgnoreCount>
	struct split_impl :
		split_impl <
			typename push_back< Head, typename front< Tail>::type >::type,
			typename pop_front<Tail>::type,
			Index - 1, IgnoreCount
	> {};


	template<typename Head, typename Tail, uint32 IgnoreCount>
	struct split_impl<Head, Tail, 0, IgnoreCount>
	{
		typedef Head head;
		typedef typename pop_front_n<Tail, IgnoreCount>::type tail;
		typedef typename merge<head, tail>::type type;
	};
}

/* split Container at Index to head and tail, ingore IngoreCount element of tail  */
template<typename Container, uint32 Index, uint32 IgnoreCount = 0>
struct split : mct_detail::split_impl< typename clear<Container>::type, Container, Index, IgnoreCount>
{};

template<typename Container, uint32 Index, uint32 IgnoreCount = 0>
using split_head = typename split<Container, Index, IgnoreCount>::head;
 
template<typename Container, uint32 Index, uint32 IgnoreCount = 0>
using split_tail = typename split<Container, Index, IgnoreCount>::tail;

/* split Args... */
template<uint32 Index, uint32 IgnoreCount, typename... Args>
using split_args = split< arg_pack<Args...>, Index, IgnoreCount>;

template<uint32 Index, typename... Args>
using args_head = typename split_args<Index, 0, Args...>::head;

template<uint32 Index , typename... Args>
using args_tail = typename split_args<Index, 0, Args...>::tail;

template<uint32 Index, uint32 IgnoreCount, typename... Args>
using args_erase = typename split_args<Index, IgnoreCount, Args...>::type;

ccdk_namespace_mpl_end