#pragma once

#include<core/preprocessor/sign.h>
#include<core/preprocessor/bool_if.h>

#define A3D_PP_IF_COMMA(Condi) A3D_PP_IF(Condi, A3D_PP_COMMA, A3D_PP_NULL)