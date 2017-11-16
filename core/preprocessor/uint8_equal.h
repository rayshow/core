#pragma once

#include<core/preprocessor/impl/uint8_to_byte.h>
#include<core/preprocessor/impl/byte_equal.h>

//x, y are in range [0, 255]
#define CCDK_PP_EQUAL(x, y) CCDK_PP_BYTE_EQUAL( CCDK_PP_INT_BYTE(x), CCDK_PP_INT_BYTE(y))
