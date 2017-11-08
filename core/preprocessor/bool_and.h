#pragma once

//first, second ranges [0, 1]
#define A3D_PP_AND(x, y)      A3D_PP_AND_FAST(x, y)
#define A3D_PP_AND_FAST(x, y) A3D_PP_AND_VALUE ## x ## y

#define A3D_PP_AND_VALUE00 0
#define A3D_PP_AND_VALUE01 0
#define A3D_PP_AND_VALUE10 0
#define A3D_PP_AND_VALUE11 1
