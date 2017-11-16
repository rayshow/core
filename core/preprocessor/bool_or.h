#pragma once

//x, y ranges [0, 1]
#define CCDK_PP_OR(x, y)       CCDK_PP_OR_FAST(x, y)
#define CCDK_PP_OR_FAST(x, y)  CCDK_PP_OR_VALUE ## x ## y
#define CCDK_PP_OR_VALUE00 0
#define CCDK_PP_OR_VALUE01 1
#define CCDK_PP_OR_VALUE10 1
#define CCDK_PP_OR_VALUE11 1
