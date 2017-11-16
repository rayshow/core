#pragma once

//evaluate text for case that text is another macro-define
#define CCDK_PP_CONNECT(x, y) CCDK_PP_CONNECT_HELPER(x, y)
#define CCDK_PP_CONNECT_HELPER(x, y) x y

#define CCDK_PP_CONNECT_REPARSE(x, y) CCDK_PP_CONNECT_REPARSE_STEP1(x, y)
#define CCDK_PP_CONNECT_REPARSE_STEP1(x, y) CCDK_PP_CONNECT_REPARSE_STEP2(,x##y)
#define CCDK_PP_CONNECT_REPARSE_STEP2(_,result) _##result

#define CCDK_PP_CONNECT3(x, y, z) CCDK_PP_CONNECT3_HELPER(x, y, z)
#define CCDK_PP_CONNECT3_HELPER(x, y, z) x ## y ## z

#define CCDK_PP_CONNECT4(x,y,z,w) CCDK_PP_CONNECT4_HELPER(x, y, z,w)
#define CCDK_PP_CONNECT4_HELPER(x, y, z,w) x ## y ## z ## w
