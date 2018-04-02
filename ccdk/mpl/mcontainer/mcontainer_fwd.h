#pragma once

#include<ccdk/mpl/base/type_.h>
#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/iterator/iterator_fwd.h>

ccdk_namespace_mpl_start
/* container */
template<typename... Args> struct arg_pack;

template<typename T, T... args> struct val_pack;

ccdk_namespace_mpl_end