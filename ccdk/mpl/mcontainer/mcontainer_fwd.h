#pragma once

#include<ccdk/mpl/base/type_.h>
#include<ccdk/mpl/mpl_module.h>

ccdk_namespace_mpl_start

/* mconainter category */
struct forward_category { };
struct bidirection_categroy : public forward_category {};
struct random_category : bidirection_categroy {};

template<typename Container>
struct front_;

/* iterator begin*/
template<typename Container>
struct begin_;

/* iterator begin*/
template<typename Container>
struct end_;

/* iterator next */
template<typename It>
struct next_;

/* iterator deref */
template<typename It>
struct deref_;

/* get type at Index pos */
template<typename Container, uint32 Index>
struct at_;

template<typename Container>
struct pop_front_;

template<typename Container, uint32 N>
struct pop_front_n_;

template<typename Container>
struct pop_back_;

template<typename Container,typename P>
struct push_front_;

template<typename Container, typename P>
struct push_back_;

template<typename Container>
struct clear_;


/* container */
template<typename... Args> struct arg_pack;

template<typename T, T... args> struct val_pack;

template<typename... Args>
struct vector_;

ccdk_namespace_mpl_end