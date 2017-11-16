#pragma once

//first, second ranges [0, 1]
#define CCDK_PP_AND(x, y)      CCDK_PP_AND_FAST(x, y)
#define CCDK_PP_AND_FAST(x, y) CCDK_PP_AND_VALUE ## x ## y

#define CCDK_PP_AND_VALUE00 0
#define CCDK_PP_AND_VALUE01 0
#define CCDK_PP_AND_VALUE10 0
#define CCDK_PP_AND_VALUE11 1
