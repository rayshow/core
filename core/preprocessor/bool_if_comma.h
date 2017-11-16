#pragma once

#include<core/preprocessor/sign.h>
#include<core/preprocessor/bool_if.h>

#define CCDK_PP_IF_COMMA(Condi) CCDK_PP_IF(Condi, CCDK_PP_COMMA, CCDK_PP_NULL)
