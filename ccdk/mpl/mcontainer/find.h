#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/type_traits/is_same.h>
#include<ccdk/mpl/type_traits/identity.h>
#include<ccdk/mpl/base/derive_if.h>
#include<ccdk/mpl/mcontainer/mcontainer_fwd.h>

ccdk_namespace_mpl_start

template<typename It, typename T, int Index>
struct find_at_impl: derive_if<  is_same< typename deref<It>::type, T>, identity<T>, find_at_impl< typename next<It>::type, T, Index+1>>
{};


template<typename Container, typename T> 
struct find_at : find_at_impl < typedef begin<Container>::type, T, 0>
{};

template<typename Container, typename T>
using find_at_t = typename find_at<Container, T>::type;

ccdk_namespace_mpl_end