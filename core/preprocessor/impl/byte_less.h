#pragma once
#include<core/preprocessor/bool_if.h>
#include<core/preprocessor/seq_unpack.h>
#include<core/preprocessor/impl/bit4x4_equal.h>
#include<core/preprocessor/impl/bit4x4_less.h>

#define A3D_PP_BYTE_LESS(byte1, byte2) A3D_PP_BYTE_LESS_CONNECT_REPARSE(A3D_PP_BYTE_LESS_EXPLICIT,(A3D_PP_UNPACK(byte1), A3D_PP_UNPACK(byte2)))

//compare x7x6x5x4 with y7y6y5y4, if         x7x6x5x4 == y7y6y5y4   goto compare x3x2x1x0 with y3y2y1y0
//                                else  if   x7x6x5x4 <  y7y6y5y4   return 1 
//                                else       x7x6x5x4 >  y7y6y5y4   return 0
//compare x3x2x1x0 with y3y2y1y0, if         x3x2x1x0 < y3y2y1y0    return 1, else return 0
#define A3D_PP_BYTE_LESS_EXPLICIT(x7, x6, x5, x4, x3, x2, x1, x0,y7, y6, y5, y4, y3, y2, y1, y0) \
			A3D_PP_IF( A3D_PP_BIT4x4_EQUAL(x7,x6,x5,x4,y7,y6,y5,y4),                             \
				A3D_PP_BYTE_LESS_L4BIT_LESS,                                                     \
				A3D_PP_BYTE_LESS_H4BIT_LESS)(x7,x6,x5,x4,x3,x2,x1,x0,y7,y6,y5,y4,y3,y2,y1,y0)

#define A3D_PP_BYTE_LESS_H4BIT_LESS(x7, x6, x5, x4,_0,_1,_2,_3, y7, y6, y5, y4,_4,_5,_6,_7)  A3D_PP_BIT4x4_LESS(x7,x6,x5,x4,y7,y6,y5,y4)
#define A3D_PP_BYTE_LESS_L4BIT_LESS(_0,_1,_2,_3, x3, x2, x1, x0,_4,_5,_6,_7, y3, y2, y1, y0) A3D_PP_BIT4x4_LESS(x3,x2,x1,x0,y3,y2,y1,y0)

#define A3D_PP_BYTE_LESS_CONNECT_REPARSE(x, y) A3D_PP_BYTE_LESS_CONNECT_REPARSE_STEP1(x, y)
#define A3D_PP_BYTE_LESS_CONNECT_REPARSE_STEP1(x, y) A3D_PP_BYTE_LESS_CONNECT_REPARSE_STEP2(x y)
#define A3D_PP_BYTE_LESS_CONNECT_REPARSE_STEP2(result) result




