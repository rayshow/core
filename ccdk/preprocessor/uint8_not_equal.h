#pragma once

#include<ccdk/preprocessor/uint8_equal.h>
#include<ccdk/preprocessor/bool_not.h>

// x, y in range [0,255]
#define CCDK_PP_NEQUAL( x, y) CCDK_PP_NOT( CCDK_PP_EQUAL(x,y) )
