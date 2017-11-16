#pragma once

#include<core/preprocessor/impl/byte_shift_right.h>
#include<core/preprocessor/impl/uint8_to_byte.h>
#include<core/preprocessor/impl/byte_to_uint8.h>
#include<core/preprocessor/impl/byte_to_bool.h>

#define CCDK_PP_SHIFT_RIGHT(value, n)       CCDK_PP_BYTE_INT( CCDK_PP_BYTE_SHIFT_RIGHT( CCDK_PP_INT_BYTE(value),n ))
#define CCDK_PP_SHIFT_RIGHT_BOOL(value, n)  CCDK_PP_BYTE_BOOL( CCDK_PP_BYTE_SHIFT_RIGHT( CCDK_PP_INT_BYTE(value),n ))
