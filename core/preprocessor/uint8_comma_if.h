#pragma once

#include<core/preprocessor/bool_if.h>
#include<core/preprocessor/seq_unpack.h>
#include<core/preprocessor/sign.h>
#include<core/preprocessor/impl/uint8_to_bool.h>

#define CCDK_PP_INT_COMMA_IF(Condi) CCDK_PP_IF(CCDK_PP_INT_BOOL(Condi), CCDK_PP_COMMA,CCDK_PP_NULL)()
