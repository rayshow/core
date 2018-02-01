#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/base/apply_.h>

ccdk_namespace_mpl_start

template<template<typename,typename> typename Binary, typename Prev, typename... Args>
struct fold_args : Binary < Prev, fold_args< Binary, Args...>> {};

template<template<typename,typename> typename Binary, typename Prev>
struct fold_args<Binary, Prev>: Prev {};

ccdk_namespace_mpl_end