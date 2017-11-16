#pragma once

#include<core/preprocessor/impl/byte_less.h>
#include<core/preprocessor/impl/uint8_to_byte.h>
#include<core/preprocessor/bool_not.h>

//x <= y => !(x > y) => !(y < x)
#define CCDK_PP_LEQUAL(x, y) CCDK_PP_NOT( CCDK_PP_BYTE_LESS( CCDK_PP_INT_BYTE(y),CCDK_PP_INT_BYTE(x)))
