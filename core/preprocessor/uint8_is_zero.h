#pragma once
#include<core/preprocessor/impl/uint8_to_bool.h>
#include<core/preprocessor/bool_not.h>

//x==0 return 1
//x>0  return 0
#define A3D_PP_IS_ZERO(x) A3D_PP_NOT( A3D_PP_INT_BOOL(x) )