#pragma once

#include<core/preprocessor/impl/byte_less.h>
#include<core/preprocessor/impl/uint8_to_byte.h>

#define A3D_PP_LESS(x, y)    A3D_PP_BYTE_LESS(A3D_PP_INT_BYTE(x), A3D_PP_INT_BYTE(y) )


