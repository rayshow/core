#pragma once

#include<ccdk/preprocessor/impl/byte_less.h>
#include<ccdk/preprocessor/impl/uint8_to_byte.h>

#define CCDK_PP_LESS(x, y)    CCDK_PP_BYTE_LESS(CCDK_PP_INT_BYTE(x), CCDK_PP_INT_BYTE(y) )


