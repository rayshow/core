#pragma once

//first, second ranges [0, 1]
#define A3D_PP_BOOL_EQUAL(x, y)      A3D_PP_BOOL_EQUAL_FAST(x, y)
#define A3D_PP_BOOL_EQUAL_FAST(x, y) A3D_PP_BOOL_EQUAL_VALUE ## x ## y
#define A3D_PP_BOOL_EQUAL_VALUE00 1
#define A3D_PP_BOOL_EQUAL_VALUE01 0
#define A3D_PP_BOOL_EQUAL_VALUE10 0
#define A3D_PP_BOOL_EQUAL_VALUE11 1