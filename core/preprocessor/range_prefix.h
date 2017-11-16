#pragma once
#include<core/preprocessor/range_inner_call.h>
#include<core/preprocessor/seq_unpack.h>
#include<core/preprocessor/uint8_if.h>
#include<core/preprocessor/ingore.h>

#define CCDK_PP_RANGE_PREFIX_CONNECT(i, count_down, prefix, split) \
			prefix ## i CCDK_PP_INT_IF( count_down, CCDK_PP_UNPACK, CCDK_PP_INGORE)(split)

// for(int i=start; i<=end; ++i)
//      prefix ## i if(i!=end) split
#define  CCDK_PP_RANGE_PREFIX(prefix, start, end, split)              \
			CCDK_PP_RANGE_INNER_CALL(start, end, 1,                   \
				CCDK_PP_RANGE_PREFIX_CONNECT, prefix, split)

// for(int i=0; i<=end; i+=step)
//      prefix ## i if(i!=end) ## split
#define CCDK_PP_RANGE_PREFIX_STEP(prefix, start, end, step, split)    \
			CCDK_PP_RANGE_INNER_CALL(start, end, step,                \
				CCDK_PP_RANGE_PREFIX_CONNECT, prefix, split)
			