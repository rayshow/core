#pragma once

//x ranges [0,1]
#define CCDK_PP_NOT(x) CCDK_PP_NOT_FAST(x)
#define CCDK_PP_NOT_FAST(x) CCDK_PP_NOT_VALUE##x
#define CCDK_PP_NOT_VALUE0 1
#define CCDK_PP_NOT_VALUE1 0
