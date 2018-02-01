#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/mcontainer/mcontainer_fwd.h>

ccdk_namespace_mpl_start

template<typename T, typename U>
struct sub_ : T::template sub<U>{};

ccdk_namespace_mpl_end