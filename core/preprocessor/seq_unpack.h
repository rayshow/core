#pragma once

//( content... ) =>  content...
//unpack __VA_ARGS__
#define A3D_PP_UNPACK( seq ) A3D_PP_UNPACK_CONNECT(A3D_PP_UNPACK_HELPER, seq)
#define A3D_PP_UNPACK_HELPER(...) __VA_ARGS__

#define A3D_PP_UNPACK_CONNECT(x, y)  A3D_PP_UNPACK_CONNECT2(x, y)
#define A3D_PP_UNPACK_CONNECT2(x, y) A3D_PP_UNPACK_CONNECT3(x y)
#define A3D_PP_UNPACK_CONNECT3(result) result
