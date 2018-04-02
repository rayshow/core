#pragma once

#include<ccdk/mpl/type_traits/impl/has_attribute_decl.h>

ccdk_namespace_mpl_start

// test wheather T().next is well-formed and is type T*
CCDK_TT_HAS_ATTRIBUTE_DECL1(next);

// test wheather T().prev is well-formed and is type T*
CCDK_TT_HAS_ATTRIBUTE_DECL1(prev);

template<typename T>
struct is_forward_node : has_attribute_next<T> {};

template<typename T>
constexpr is_forward_node<T> is_forward_node_c{};

template<typename T>
struct is_biward_node : and_< has_attribute_next<T>, has_attribute_prev<T>> {};

template<typename T>
struct is_biward_node<T> is_biward_node_c {};

ccdk_namespace_mpl_end