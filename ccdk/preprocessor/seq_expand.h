#pragma once

#define CCDK_PP_EXPAND_UNPACK(...) __VA_ARGS__


//(arg1,arg2,arg3,...) + __VA_ARGS_ => (arg1,arg2,arg3,...,__VA_ARGS__)
// size(arg...) <= 63 because of compile limit
#define CCDK_PP_EXPAND(pair, ...)       CCDK_PP_EXPAND_STEP1(pair, __VA_ARGS__)
#define CCDK_PP_EXPAND_STEP1(pair, ...) CCDK_PP_EXPAND_STEP2(CCDK_PP_EXPAND_UNPACK pair, __VA_ARGS__) 
#define CCDK_PP_EXPAND_STEP2(...)       (__VA_ARGS__)
