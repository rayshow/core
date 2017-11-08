#pragma once

#include<core/preprocessor/impl/byte_mul.h>
#include<core/preprocessor/impl/byte_to_uint8.h>
#include<core/preprocessor/impl/uint8_to_byte.h>

#define A3D_PP_MUL(x, y) A3D_PP_BYTE_INT( A3D_PP_BYTE_MUL( A3D_PP_INT_BYTE(x),A3D_PP_INT_BYTE(y))  )