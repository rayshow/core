#pragma once
#include<ccdk/preprocessor/range_inner_call.h>
#include<ccdk/preprocessor/seq_unpack.h>
#include<ccdk/preprocessor/uint8_if.h>
#include<ccdk/preprocessor/ingore.h>
#include<ccdk/preprocessor/connect.h>

// for(int i=start; i<=end; ++i)
//      prefix ## i ## suffix  second ## i  split
#define  CCDK_PP_RANGE_ALIAS(prefix, suffix, second, start, end, split)               \
			CCDK_PP_RANGE_INNER_CALL(start, end, 1,                                   \
				CCDK_PP_RANGE_ALIAS_CONNECT, prefix, suffix, second, split)

// for(int i=start; i<=end; ++i)
//      first ## i-1 ## suffix ## second ## i ## split
#define CCDK_PP_RANGE_CHAIN_ALIAS(prefix, suffix, second, start, end, split)     \
			CCDK_PP_RANGE_INNER_CALL(start, end, 1,                              \
				CCDK_PP_RANGE_CHAIN_ALIAS_CONNECT, prefix, suffix, second, split)

#define CCDK_PP_RANGE_ALIAS_CONNECT3(x, y, z) CCDK_PP_RANGE_ALIAS_CONNECT3_STEP1(x, y, z)
#define CCDK_PP_RANGE_ALIAS_CONNECT3_STEP1(x, y,z) CCDK_PP_RANGE_ALIAS_CONNECT3_STEP2(x##y z)
#define CCDK_PP_RANGE_ALIAS_CONNECT3_STEP2(result) result
 
#define CCDK_PP_RANGE_ALIAS_CONNECT(i, _, prefix, suffix, second, split) \
			prefix ## i suffix second ## i CCDK_PP_UNPACK(split)

#define CCDK_PP_RANGE_CHAIN_ALIAS_CONNECT(i, _, prefix, suffix, second, split) \
			CCDK_PP_RANGE_ALIAS_CONNECT3(prefix, CCDK_PP_SUB1(i), suffix) second ## i  CCDK_PP_UNPACK(split)
