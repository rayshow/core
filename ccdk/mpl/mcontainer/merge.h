#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/mcontainer/mcontainer_fwd.h>

ccdk_namespace_mpl_start

/* merge 2 Container */
template<typename Container, typename Container2>
struct merge : Container::template merge<Container2>{};

template<typename Container, typename Container2>
using merge_t = typename merge<Container, Container2>::type;

ccdk_namespace_mpl_end