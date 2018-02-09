#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/mcontainer/mcontainer_fwd.h>

ccdk_namespace_mpl_start

/* clear */
template<typename Container> struct clear_ { typedef typename Container::clear type; };
template<typename Container> using clear_t = typename  Container::clear;

/* empty */
template<typename Container> struct empty_ :Container::empty {};

/* size */
template<typename Container> struct size_ :Container::size {};
template<typename Container> constexpr uint32 size_v = size_<Container>::value;

/* merge */
template<typename Container, typename Container2> struct merge_ : Container::template merge<Container2>{};
template<typename Container, typename Container2> using merge_t = typename merge_<Container, Container2>::type;

/* split */
template<typename Container, int32 Index> struct split_ : Container::template split<Index>{};
template<typename Container, int32 Index> using split_head_t = typename split_<Container,Index>::split_head;
template<typename Container, int32 Index> using split_tail_t = typename split_<Container,Index>::split_tail;



ccdk_namespace_mpl_end