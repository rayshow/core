#pragma once

#include<core/preprocessor/impl/byte_less.h>
#include<core/preprocessor/impl/uint8_to_byte.h>
#include<core/preprocessor/bool_not.h>

//x >= y => !(x < y)
#define A3D_PP_GEQUAL(x, y) A3D_PP_NOT( A3D_PP_BYTE_LESS( A3D_PP_INT_BYTE(x),A3D_PP_INT_BYTE(y)))