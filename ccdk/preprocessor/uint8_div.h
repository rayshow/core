#pragma once

#include<ccdk/preprocessor/impl/byte_div.h>
#include<ccdk/preprocessor/impl/byte_to_uint8.h>
#include<ccdk/preprocessor/impl/uint8_to_byte.h>

#define CCDK_PP_DIV(x,y) CCDK_PP_BYTE_INT( CCDK_PP_BYTE_DIV( CCDK_PP_INT_BYTE(x), CCDK_PP_INT_BYTE(y) ) )




