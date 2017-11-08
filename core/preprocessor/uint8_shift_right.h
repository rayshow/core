#pragma once

#include<core/preprocessor/impl/byte_shift_right.h>
#include<core/preprocessor/impl/uint8_to_byte.h>
#include<core/preprocessor/impl/byte_to_uint8.h>
#include<core/preprocessor/impl/byte_to_bool.h>

#define A3D_PP_SHIFT_RIGHT(value, n)       A3D_PP_BYTE_INT( A3D_PP_BYTE_SHIFT_RIGHT( A3D_PP_INT_BYTE(value),n ))
#define A3D_PP_SHIFT_RIGHT_BOOL(value, n)  A3D_PP_BYTE_BOOL( A3D_PP_BYTE_SHIFT_RIGHT( A3D_PP_INT_BYTE(value),n ))