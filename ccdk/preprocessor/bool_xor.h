#pragma once

//x, y ranges [0, 1]
#define CCDK_PP_XOR(x, y)      CCDK_PP_XOR_FAST(x, y)
#define CCDK_PP_XOR_FAST(x, y) CCDK_PP_XOR_VALUE ## x ## y
#define CCDK_PP_XOR_VALUE00 0
#define CCDK_PP_XOR_VALUE01 1
#define CCDK_PP_XOR_VALUE10 1
#define CCDK_PP_XOR_VALUE11 0
