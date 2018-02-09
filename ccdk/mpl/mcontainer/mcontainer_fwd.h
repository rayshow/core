#pragma once

#include<ccdk/mpl/base/type_.h>
#include<ccdk/mpl/mpl_module.h>

ccdk_namespace_mpl_start

/* mconainter category */
struct forward_category { };
struct bidirection_categroy : public forward_category {};
struct random_category : bidirection_categroy {};


/* container */
template<typename... Args> struct arg_pack;

template<typename T, T... args> struct val_pack;

ccdk_namespace_mpl_end