#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/mcontainer/mcontainer_fwd.h>

ccdk_namespace_mpl_start

template<typename T, typename U>
struct add_ : T::template add<U>{};


ccdk_namespace_mpl_end