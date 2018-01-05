#pragma once

#include<ccdk/mpl/mcontainer/val_pack.h>
#include<ccdk/mpl/base/integer_.h>
#include<ccdk/mpl/base/derive_if.h>


namespace ccdk
{
	namespace mpl
	{
#define ccdk_tt_rank_single_val    0
#define ccdk_tt_rank_and_val       1
#define ccdk_tt_rank_or_val        2
#define ccdk_tt_rank_not_val       3
#define ccdk_tt_rank_not_and_val   4
#define ccdk_tt_rank_default_val   5

#define ccdk_tt_rank_is_void         1
#define ccdk_tt_rank_is_function     2
#define ccdk_tt_rank_is_mfn_ptr      4
#define ccdk_tt_rank_is_lref         5
#define ccdk_tt_rank_is_rref         6
#define ccdk_tt_rank_is_class        7
#define ccdk_tt_rank_is_pointer      8
#define ccdk_tt_rank_is_mfn_pointer  9
#define ccdk_tt_rank_is_integer      10
#define ccdk_tt_rank_is_float        11

#define ccdk_tt_rank_has_invoker     50

		template<template<typename...> typename T>
		struct case_;

		template<template<typename...> typename T>
		struct case_index;

		template<typename T, bool>
		struct case_val;

		template<uint32 tag, typename T, template<typename...> typename... Args>
		struct rank_pack
		{
			typedef indice_pack< tag, case_index<Args>::value... > type;
		};

#define CCDK_TT_SINGLE_RANK( name) \
		template<> struct case_index<name>                                \
		{   static const uint32 value = ccdk_tt_rank_ ## name;            \
			typedef indice_pack< ccdk_tt_rank_single_val, value > type;}; \
		template<typename T> struct case_val< name<T>,true>               \
		:derive_if< name<T>, indice_pack< ccdk_tt_rank_single_val,        \
			ccdk_tt_rank_ ## name >,                                      \
			indice_pack<ccdk_tt_rank_default_val, 0>>{};                  \
		template<typename T> struct case_val< name<T>, false>             \
			:indice_pack< ccdk_tt_rank_single_val, ccdk_tt_rank_ ## name > {};

#define CCDK_TT_COMPOSE_RANK( t, name, ...) \
		template<> struct case_index<name>: rank_pack<t, void,__VA_ARGS__>{};   \
		template<typename T> struct case_val< name<T>, true>                    \
		:derive_if< name<T>, rank_pack<t, T,__VA_ARGS__>,                       \
			indice_pack<ccdk_tt_rank_default_val, 0>>{};                        \
		template<typename T> struct case_val< name<T>, false>                   \
			:rank_pack<t, T, __VA_ARGS__>{};
	}
}
