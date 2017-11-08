#pragma once

#include<core/preprocessor/impl/byte_equal.h>
#include<core/preprocessor/bool_not.h>

#define A3D_PP_BYTE_NOT_EQUAL( byte1, byte2 ) A3D_PP_NOT( A3D_PP_BYTE_EQUAL(byte1, byte2) )
