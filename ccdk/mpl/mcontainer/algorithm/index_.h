#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/base/type_.h>
#include<ccdk/mpl/base/apply_.h>
#include<ccdk/mpl/base/derive_if.h>
#include<ccdk/mpl/type_traits/is_same.h>
#include<ccdk/mpl/mcontainer/mcontainer_fwd.h>
#include<ccdk/mpl/mcontainer/iterator_.h>


ccdk_namespace_mpl_start

namespace mpl_impl
{
	template<typename It, typename T, uint32 Index>
	struct index_impl : 
			derive_if< 
				is_same< deref_t<It>, T >,
				uint32_< Index >, 
				index_impl< next_t<It>, T, Index + 1>>
	{};


	template<typename It, CCDK_MPL_TFN(Pred), uint32 Index>
	struct index_if_impl:
			derive_if< 
				Pred< deref_t<It>>,
				uint32_<Index>, 
				index_if_impl< next_t<It>, Pred, Index + 1>>
	{};
}

template<typename Container, typename T> 
struct index_ : mpl_impl::index_impl < begin_t<Container> , T, 0 >{};

template<typename Container, typename Pred>
struct index_if_ : mpl_impl::index_if_impl< begin_t<Container>, Pred, 0> {};

template<typename Container, typename T>
using index_t = typename index_<Container, T>::type;

template<typename Container, typename Pred>
using index_if_t = typename index_if_<Container, Pred>::type;

template<typename Container, typename T>
static constexpr uint32 index_v = index_<Container, T>::value;

template<typename Container, typename Pred>
static constexpr uint32 index_if_v = index_if_<Container, Pred>::value;

template<typename T, typename... Args>
static constexpr uint32 args_index_v = index_< arg_pack<Args...>, T>::value;

ccdk_namespace_mpl_end