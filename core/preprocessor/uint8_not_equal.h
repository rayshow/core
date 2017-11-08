#pragma once

#include<core/preprocessor/uint8_equal.h>
#include<core/preprocessor/bool_not.h>

// x, y in range [0,255]
#define A3D_PP_NEQUAL( x, y) A3D_PP_NOT( A3D_PP_EQUAL(x,y) )