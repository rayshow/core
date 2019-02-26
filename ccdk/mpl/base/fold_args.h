#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/base/apply_.h>

ccdk_namespace_mpl_start

template<template<class,class> class Binary, typename Prev, typename... Args>
struct fold_args : Binary < Prev, fold_args< Binary, Args...>> {};

template<template<class,class> class Binary, typename Prev>
struct fold_args<Binary, Prev>: Prev {};

template<template<class, class> class Binary, typename Prev, typename... Args>
struct reverse_fold_args;

template<template<class, class> class Binary, typename Prev, typename T, typename... Args>
struct reverse_fold_args<Binary, Prev, T, Args...>
		: reverse_fold_args< Binary, Binary<Prev, T>, Args...> {};

template<template<class, class> class Binary, typename Prev>
struct reverse_fold_args<Binary, Prev> : Prev {};


ccdk_namespace_mpl_end
