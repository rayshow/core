#pragma once

#include<ccdk/preprocessor/impl/uint8_to_bool.h>
#include<ccdk/preprocessor/bool_if.h>

// Value range [0,255]
#define CCDK_PP_INT_IF( Value, True, False ) CCDK_PP_IF( CCDK_PP_INT_BOOL( Value ), True, False)
 