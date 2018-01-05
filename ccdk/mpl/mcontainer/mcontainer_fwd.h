#pragma once

#include<ccdk/mpl/mpl_module.h>


ccdk_namespace_mpl_start

/* mconainter category */
struct forward_category {};

struct bidirection_categroy : public forward_category {};

struct random_category : bidirection_categroy {};


/* iterator begin*/
template<typename It>
struct begin;

/* iterator begin*/
template<typename It>
struct begin;

/* iterator next */
template<typename It>
struct next;

/* iterator deref */
template<typename It>
struct deref;

/* get type at Index pos */
template<typename Container, uint32 Index>
struct at;


template<typename... Args> struct arg_pack;

template<typename T, T... args> struct val_pack;

ccdk_namespace_mpl_end