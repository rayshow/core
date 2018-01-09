#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/mcontainer/mcontainer_fwd.h>

ccdk_namespace_mpl_start

template<typename Container, typename P>
struct push_front_ : Container::template push_front<P>
{};

ccdk_namespace_mpl_end