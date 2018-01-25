#pragma once

#include<ccdk/compile.h>
#include<ccdk/type.h>

#define ccdk_namespace_end_of_3  }}}
#define ccdk_namespace_end_of_2  }}

//module base / type_traits
#define ccdk_namespace_mpl_start     namespace ccdk{ namespace mpl{
#define ccdk_namespace_mpl_end       ccdk_namespace_end_of_2

#define ccdk_namespace_typetraits_impl_start namespace ccdk{ namespace mpl { namespace impl{
#define ccdk_namespace_typetraits_impl_end ccdk_namespace_end_of_3

#define ccdk_namespace_mpl_mct_start namespace ccdk{ namespace mpl{ namespace mct{
#define ccdk_namespace_mpl_mct_end   ccdk_namespace_end_of_3

//module function
#define ccdk_namespace_mpl_fn_start  namespace ccdk{ namespace mpl{ namespace fn{
#define ccdk_namespace_mpl_fn_end    ccdk_namespace_end_of_3

//module iterator
#define ccdk_namespace_mpl_it_start  namespace ccdk{ namespace mpl{ namespace it{
#define ccdk_namespace_mpl_it_end    ccdk_namespace_end_of_3

//module fusion
#define ccdk_namespace_mpl_fs_start  namespace ccdk{ namespace mpl{ namespace fs{
#define ccdk_namespace_mpl_fs_end    ccdk_namespace_end_of_3

//module util 
#define ccdk_namespace_mpl_util_start namespace ccdk{ namespace mpl{ namespace util{
#define ccdk_namespace_mpl_util_end  ccdk_namespace_end_of_3 

//module smart_ptr
#define ccdk_namespace_mpl_sp_start namespace ccdk{ namespace mpl{ namespace sp{
#define ccdk_namespace_mpl_sp_end  ccdk_namespace_end_of_3 

//module unit
#define ccdk_namespace_mpl_units_start namespace ccdk{ namespace mpl{ namespace units{
#define ccdk_namespace_mpl_units_end  ccdk_namespace_end_of_3 