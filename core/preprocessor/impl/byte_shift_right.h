#pragma once

#define A3D_PP_BYTE_SHIFT_RIGHT( byte, n ) A3D_PP_BYTE_SHIFT_RIGHT_FAST( byte, n)
#define A3D_PP_BYTE_SHIFT_RIGHT_FAST(byte, n) A3D_PP_BYTE_SHIFT_RIGHT_REPARSE(A3D_PP_BYTE_SHIFT_RIGHT_VALUE ## n byte)
#define A3D_PP_BYTE_SHIFT_RIGHT_REPARSE(...) __VA_ARGS__

#define A3D_PP_BYTE_SHIFT_RIGHT_VALUE0( b7, b6, b5, b4, b3, b2, b1, b0) (b7, b6, b5, b4, b3, b2, b1, b0)
#define A3D_PP_BYTE_SHIFT_RIGHT_VALUE1( b7, b6, b5, b4, b3, b2, b1, b0) (0,b7,b6,b5,b4,b3,b2,b1)
#define A3D_PP_BYTE_SHIFT_RIGHT_VALUE2( b7, b6, b5, b4, b3, b2, b1, b0) (0,0,b7,b6,b5,b4,b3,b2)
#define A3D_PP_BYTE_SHIFT_RIGHT_VALUE3( b7, b6, b5, b4, b3, b2, b1, b0) (0,0,0,b7,b6,b5,b4,b3)
#define A3D_PP_BYTE_SHIFT_RIGHT_VALUE4( b7, b6, b5, b4, b3, b2, b1, b0) (0,0,0,0,b7,b6,b5,b4)
#define A3D_PP_BYTE_SHIFT_RIGHT_VALUE5( b7, b6, b5, b4, b3, b2, b1, b0) (0,0,0,0,0,b7,b6,b5)
#define A3D_PP_BYTE_SHIFT_RIGHT_VALUE6( b7, b6, b5, b4, b3, b2, b1, b0) (0,0,0,0,0,0,b7,b6)
#define A3D_PP_BYTE_SHIFT_RIGHT_VALUE7( b7, b6, b5, b4, b3, b2, b1, b0) (0,0,0,0,0,0,0,b7)
#define A3D_PP_BYTE_SHIFT_RIGHT_VALUE8( b7, b6, b5, b4, b3, b2, b1, b0) A3D_PP_MOD_ZERO_ERROR

