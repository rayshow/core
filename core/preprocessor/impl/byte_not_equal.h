#pragma once

#include<core/preprocessor/impl/byte_equal.h>
#include<core/preprocessor/bool_not.h>

#define CCDK_PP_BYTE_NOT_EQUAL( byte1, byte2 ) CCDK_PP_NOT( CCDK_PP_BYTE_EQUAL(byte1, byte2) )
