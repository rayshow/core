#pragma once

#include<core/preprocessor/impl/uint8_to_bool.h>
#include<core/preprocessor/bool_if.h>

// Value range [0,255]
#define A3D_PP_INT_IF( Value, True, False ) A3D_PP_IF( A3D_PP_INT_BOOL( Value ), True, False)
 