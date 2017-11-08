#pragma once

//evaluate text for case that text is another macro-define
#define A3D_PP_CONNECT(x, y) A3D_PP_CONNECT_HELPER(x, y)
#define A3D_PP_CONNECT_HELPER(x, y) x y

#define A3D_PP_CONNECT_REPARSE(x, y) A3D_PP_CONNECT_REPARSE_STEP1(x, y)
#define A3D_PP_CONNECT_REPARSE_STEP1(x, y) A3D_PP_CONNECT_REPARSE_STEP2(,x##y)
#define A3D_PP_CONNECT_REPARSE_STEP2(_,result) _##result

#define A3D_PP_CONNECT3(x, y, z) A3D_PP_CONNECT3_HELPER(x, y, z)
#define A3D_PP_CONNECT3_HELPER(x, y, z) x ## y ## z

#define A3D_PP_CONNECT4(x,y,z,w) A3D_PP_CONNECT4_HELPER(x, y, z,w)
#define A3D_PP_CONNECT4_HELPER(x, y, z,w) x ## y ## z ## w