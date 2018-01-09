#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/base/integer_.h>
#include<ccdk/mpl/type_traits/is_same.h>
#include<ccdk/mpl/base/derive_if.h>
#include<ccdk/mpl/mcontainer/mcontainer_fwd.h>
#include<ccdk/mpl/mcontainer/deref.h>
#include<ccdk/mpl/mcontainer/begin.h>
#include<ccdk/mpl/mcontainer/next.h>

ccdk_namespace_mpl_start

namespace mct_detail
{
	template<typename It, typename T, int Index>
	struct find_impl : derive_if< is_same< typename deref_<It>::type, T>, uint32_<Index>, find_impl< typename next_<It>::type, T, Index + 1>>{};


	template<typename It, typename Pred, uint32 Index>
	struct find_if_impl : derive_if<  typename Pred::template apply< typename deref_<It>::type>, uint32_<Index>, find_if_impl< typename next_<It>::type, Pred, Index + 1>>{};
}



template<typename Container, typename T> 
struct find_ : mct_detail::find_impl < typename begin_<Container>::type, T, 0>{};

template<typename Container, typename Pred>
struct find_if_ : mct_detail::find_if_impl< typename begin_<Container>::type, Pred, 0> {};


template<typename Container, typename T>
using find_t = typename find_<Container, T>::type;

template<typename Container, typename Pred>
using find_if_t = typename find_if_<Container, Pred>::type;

template<typename Container, typename T>
static constexpr uint32 find_v = find_<Container, T>::type::value;

template<typename Container, typename Pred>
static constexpr uint32 find_if_v = find_if_<Container, Pred>::type::value;

template<typename T, typename... Args>
static constexpr uint32 args_find_v = find_< arg_pack<Args...>, T>::value;


ccdk_namespace_mpl_end