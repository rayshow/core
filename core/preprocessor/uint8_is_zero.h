#pragma once
#include<core/preprocessor/impl/uint8_to_bool.h>
#include<core/preprocessor/bool_not.h>

//x==0 return 1
//x>0  return 0
#define CCDK_PP_IS_ZERO(x) CCDK_PP_NOT( CCDK_PP_INT_BOOL(x) )
