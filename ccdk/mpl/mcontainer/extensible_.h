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

ccdk_namespace_mpl_end