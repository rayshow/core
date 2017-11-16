#pragma once

//( content... ) =>  content...
//unpack __VA_ARGS__
#define CCDK_PP_UNPACK( seq ) CCDK_PP_UNPACK_CONNECT(CCDK_PP_UNPACK_HELPER, seq)
#define CCDK_PP_UNPACK_HELPER(...) __VA_ARGS__

#define CCDK_PP_UNPACK_CONNECT(x, y)  CCDK_PP_UNPACK_CONNECT2(x, y)
#define CCDK_PP_UNPACK_CONNECT2(x, y) CCDK_PP_UNPACK_CONNECT3(x y)
#define CCDK_PP_UNPACK_CONNECT3(result) result
