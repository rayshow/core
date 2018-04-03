#pragma once

#include<ccdk/mpl/base/type_.h>
#include<ccdk/mpl/mpl_module.h>

ccdk_namespace_mpl_start

struct mforward_category {};
struct mbiward_category {};
struct mrandom_category {};

/* container */
template<typename... Args> struct arg_pack;

template<typename T, T... args> struct val_pack;

ccdk_namespace_mpl_end