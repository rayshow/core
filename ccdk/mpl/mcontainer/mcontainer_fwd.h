#pragma once

#include<ccdk/mpl/mpl_module.h>


ccdk_namespace_mpl_start

/* mconainter category */
struct forward_category {};

struct bidirection_categroy : public forward_category {};

struct random_category : bidirection_categroy {};

template<typename Container>
struct front;

/* iterator begin*/
template<typename Container>
struct begin;

/* iterator begin*/
template<typename Container>
struct end;

/* iterator next */
template<typename It>
struct next;

/* iterator deref */
template<typename It>
struct deref;

/* get type at Index pos */
template<typename Container, uint32 Index>
struct at;

template<typename Container>
struct pop_front;

template<typename Container, uint32 N>
struct pop_front_n;

template<typename Container>
struct pop_back;

template<typename Container,typename P>
struct push_front;

template<typename Container, typename P>
struct push_back;

template<typename Container>
struct clear;


template<typename... Args> struct arg_pack;

template<typename T, T... args> struct val_pack;

ccdk_namespace_mpl_end