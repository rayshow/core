#pragma once

//x, y ranges [0, 1]
#define A3D_PP_XOR(x, y)      A3D_PP_XOR_FAST(x, y)
#define A3D_PP_XOR_FAST(x, y) A3D_PP_XOR_VALUE ## x ## y
#define A3D_PP_XOR_VALUE00 0
#define A3D_PP_XOR_VALUE01 1
#define A3D_PP_XOR_VALUE10 1
#define A3D_PP_XOR_VALUE11 0