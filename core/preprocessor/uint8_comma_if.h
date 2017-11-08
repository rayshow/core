#pragma once

#include<core/preprocessor/bool_if.h>
#include<core/preprocessor/seq_unpack.h>
#include<core/preprocessor/sign.h>
#include<core/preprocessor/impl/uint8_to_bool.h>

#define A3D_PP_INT_COMMA_IF(Condi) A3D_PP_IF(A3D_PP_INT_BOOL(Condi), A3D_PP_COMMA,A3D_PP_NULL)()