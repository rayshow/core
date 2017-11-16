#pragma once

#include<ccdk/preprocessor/impl/uint8_to_byte.h>
#include<ccdk/preprocessor/impl/byte_equal.h>

//x, y are in range [0, 255]
#define CCDK_PP_EQUAL(x, y) CCDK_PP_BYTE_EQUAL( CCDK_PP_INT_BYTE(x), CCDK_PP_INT_BYTE(y))
