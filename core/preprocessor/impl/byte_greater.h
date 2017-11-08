#pragma once

#include<core/preprocessor/impl/byte_less.h>


//byte1 > byte2 === byte2 < byte1
#define A3D_PP_BYTE_GREATER(byte1, byte2) A3D_PP_BYTE_LESS(byte2, byte1)