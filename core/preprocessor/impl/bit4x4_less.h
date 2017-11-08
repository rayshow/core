#pragma once

#include<core/preprocessor/bool_and.h>
#include<core/preprocessor/bool_or.h>
#include<core/preprocessor/impl/bit4x4_equal.h>

#define A3D_PP_BIT2x2_LESS(x1,x0,y1,y0)  A3D_PP_BIT2x2_LESS_VALUE ## x1 ## x0 ## y1 ## y0
#define A3D_PP_BIT4x4_LESS( x3, x2, x1, x0, y3, y2, y1, y0)     \
		  A3D_PP_OR( A3D_PP_BIT2x2_LESS(x3,x2,y3,y2),           \
			 A3D_PP_AND( A3D_PP_BIT2x2_LESS(x1,x0,y1,y0),       \
				A3D_PP_BIT2x2_EQUAL(x3,x2,y3,y2)) )

//2x2bit less
#define A3D_PP_BIT2x2_LESS_VALUE0000    0
#define A3D_PP_BIT2x2_LESS_VALUE0001    1
#define A3D_PP_BIT2x2_LESS_VALUE0010    1
#define A3D_PP_BIT2x2_LESS_VALUE0011    1
#define A3D_PP_BIT2x2_LESS_VALUE0100    0
#define A3D_PP_BIT2x2_LESS_VALUE0101    0
#define A3D_PP_BIT2x2_LESS_VALUE0110    1
#define A3D_PP_BIT2x2_LESS_VALUE0111    1
#define A3D_PP_BIT2x2_LESS_VALUE1000    0
#define A3D_PP_BIT2x2_LESS_VALUE1001    0
#define A3D_PP_BIT2x2_LESS_VALUE1010    0
#define A3D_PP_BIT2x2_LESS_VALUE1011    1
#define A3D_PP_BIT2x2_LESS_VALUE1100    0
#define A3D_PP_BIT2x2_LESS_VALUE1101    0
#define A3D_PP_BIT2x2_LESS_VALUE1110    0
#define A3D_PP_BIT2x2_LESS_VALUE1111    0