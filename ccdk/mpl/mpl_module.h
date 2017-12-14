#pragma once

#include<ccdk/compile.h>
#include<ccdk/type.h>

#define ccdk_namespace_end_of_3  }}}
#define ccdk_namespace_end_of_2  }}

//module base / type_traits
#define ccdk_namespace_mpl_start     namespace ccdk{ namespace mpl{
#define ccdk_namespace_mpl_end       ccdk_namespace_end_of_2

//module function
#define ccdk_namespace_mpl_fn_start  namespace ccdk{ namespace mpl{ namespace fn{
#define ccdk_namespace_mpl_fn_end    ccdk_namespace_end_of_3

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
#define ccdk_namespace_mpl_unit_start namespace ccdk{ namespace mpl{ namespace unit{
#define ccdk_namespace_mpl_unit_end  ccdk_namespace_end_of_3 