#pragma once

//x, y ranges [0, 1]
#define A3D_PP_OR(x, y)       A3D_PP_OR_FAST(x, y)
#define A3D_PP_OR_FAST(x, y)  A3D_PP_OR_VALUE ## x ## y
#define A3D_PP_OR_VALUE00 0
#define A3D_PP_OR_VALUE01 1
#define A3D_PP_OR_VALUE10 1
#define A3D_PP_OR_VALUE11 1