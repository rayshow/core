#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/base/integer_.h>
#include<ccdk/mpl/type_traits/is_same.h>
#include<ccdk/mpl/base/derive_if.h>
#include<ccdk/mpl/mcontainer/mcontainer_fwd.h>
#include<ccdk/mpl/mcontainer/forward/deref_.h>
#include<ccdk/mpl/mcontainer/forward/begin_.h>
#include<ccdk/mpl/mcontainer/forward/end_.h>
#include<ccdk/mpl/mcontainer/forward/next_.h>
#include<ccdk/mpl/mcontainer/mfunction/apply_.h>

ccdk_namespace_mpl_start

namespace mct_detail
{
	template<typename It, typename T, uint32 Index>
	struct index_impl : 
			derive_if< 
				is_same< deref_t_<It>, T >,
				uint32_< Index >, 
				index_impl< next_t_<It>, T, Index + 1>>
	{};


	template<typename It, typename Pred, uint32 Index>
	struct index_if_impl:
			derive_if< 
				apply_< Pred, deref_t_<It> >,
				uint32_<Index>, 
				index_if_impl< next_t_<It>, Pred, Index + 1>>
	{};
}

template<typename Container, typename T> 
struct index_ : mct_detail::index_impl < begin_t_<Container> , T, 0 >{};

template<typename Container, typename Pred>
struct index_if_ : mct_detail::index_if_impl< begin_t_<Container>, Pred, 0> {};

template<typename Container, typename T>
using index_t_ = typename index_<Container, T>::type;

template<typename Container, typename Pred>
using index_if_t_ = typename index_if_<Container, Pred>::type;

template<typename Container, typename T>
static constexpr uint32 index_v = index_<Container, T>::type::value;

template<typename Container, typename Pred>
static constexpr uint32 index_if_v = index_if_<Container, Pred>::type::value;

template<typename T, typename... Args>
static constexpr uint32 args_index_v = index_< arg_pack<Args...>, T>::value;

ccdk_namespace_mpl_end