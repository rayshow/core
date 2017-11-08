#pragma once
#include<core/preprocessor/range_inner_call.h>
#include<core/preprocessor/seq_unpack.h>
#include<core/preprocessor/uint8_if.h>
#include<core/preprocessor/ingore.h>
#include<core/preprocessor/connect.h>

// for(int i=start; i<=end; ++i)
//      prefix ## i ## suffix  second ## i  split
#define  A3D_PP_RANGE_ALIAS(prefix, suffix, second, start, end, split)               \
			A3D_PP_RANGE_INNER_CALL(start, end, 1,                                   \
				A3D_PP_RANGE_ALIAS_CONNECT, prefix, suffix, second, split)

// for(int i=start; i<=end; ++i)
//      first ## i-1 ## suffix ## second ## i ## split
#define A3D_PP_RANGE_CHAIN_ALIAS(prefix, suffix, second, start, end, split)     \
			A3D_PP_RANGE_INNER_CALL(start, end, 1,                              \
				A3D_PP_RANGE_CHAIN_ALIAS_CONNECT, prefix, suffix, second, split)

#define A3D_PP_RANGE_ALIAS_CONNECT3(x, y, z) A3D_PP_RANGE_ALIAS_CONNECT3_STEP1(x, y, z)
#define A3D_PP_RANGE_ALIAS_CONNECT3_STEP1(x, y,z) A3D_PP_RANGE_ALIAS_CONNECT3_STEP2(x##y z)
#define A3D_PP_RANGE_ALIAS_CONNECT3_STEP2(result) result
 
#define A3D_PP_RANGE_ALIAS_CONNECT(i, _, prefix, suffix, second, split) \
			prefix ## i suffix second ## i A3D_PP_UNPACK(split)

#define A3D_PP_RANGE_CHAIN_ALIAS_CONNECT(i, _, prefix, suffix, second, split) \
			A3D_PP_RANGE_ALIAS_CONNECT3(prefix, A3D_PP_SUB1(i), suffix) second ## i  A3D_PP_UNPACK(split)
