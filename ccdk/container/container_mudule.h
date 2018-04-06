#pragma once

#include<ccdk/compile.h>
#include<ccdk/type.h>

#define ccdk_namespace_end_of_3  }}}
#define ccdk_namespace_end_of_2  }}

// container 
#define ccdk_namespace_ct_start     namespace ccdk{ namespace ct {
#define ccdk_namespace_ct_end       ccdk_namespace_end_of_2

ccdk_namespace_ct_start
struct from_string_literial {};
constexpr from_string_literial from_string_literial_c{};
ccdk_namespace_ct_end