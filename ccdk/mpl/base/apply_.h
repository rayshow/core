#pragma once

#include<ccdk/mpl/mpl_module.h>

ccdk_namespace_mpl_start

template<typename MFn, typename... Args>
struct apply_
{
	typedef typename MFn::template apply<Args...> apply_type;
	typedef typename apply_type::type    type;
};

template<typename MFn, typename... Args>
using apply_t = typename apply_<MFn,Args...>::type;


template<template<typename...> typename TFn, typename... Args>
struct tapply_
{
	typedef typename TFn<Args...>::type type;
};


ccdk_namespace_mpl_end