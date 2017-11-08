#pragma once

//x ranges [0,1]
#define A3D_PP_NOT(x) A3D_PP_NOT_FAST(x)
#define A3D_PP_NOT_FAST(x) A3D_PP_NOT_VALUE##x
#define A3D_PP_NOT_VALUE0 1
#define A3D_PP_NOT_VALUE1 0