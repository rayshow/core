#pragma once

#include<ccdk/compile.h>
#include<ccdk/mpl/mcontainer/mcontainer_fwd.h>


ccdk_namespace_mpl_start

template<typename Container, CCDK_MPL_TFN1(Filter)>
struct filter_view_
{

};

ccdk_namespace_mpl_end