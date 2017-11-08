#pragma once

#define A3D_PP_EXPAND_UNPACK(...) __VA_ARGS__


//(arg1,arg2,arg3,...) + __VA_ARGS_ => (arg1,arg2,arg3,...,__VA_ARGS__)
// size(arg...) <= 63 because of compile limit
#define A3D_PP_EXPAND(pair, ...)       A3D_PP_EXPAND_STEP1(pair, __VA_ARGS__)
#define A3D_PP_EXPAND_STEP1(pair, ...) A3D_PP_EXPAND_STEP2(A3D_PP_EXPAND_UNPACK pair, __VA_ARGS__) 
#define A3D_PP_EXPAND_STEP2(...)       (__VA_ARGS__)