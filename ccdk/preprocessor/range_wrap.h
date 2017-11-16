#pragma once
#include<ccdk/preprocessor/range_inner_call.h>
#include<ccdk/preprocessor/seq_unpack.h>
#include<ccdk/preprocessor/uint8_if.h>
#include<ccdk/preprocessor/ingore.h>

#define CCDK_PP_RANGE_WRAP_CONNECT(i, count_down, prefix, suffix, split) \
			prefix ## i suffix CCDK_PP_INT_IF( count_down, CCDK_PP_UNPACK, CCDK_PP_INGORE)(split)

// for(int i=start; i<=end; ++i)
//      prefix ## i ## suffix if(i!=end) ##split
#define  CCDK_PP_RANGE_WRAP(prefix, start, end, suffix, split)                \
			CCDK_PP_RANGE_INNER_CALL(start, end, 1,                           \
				CCDK_PP_RANGE_WRAP_CONNECT, prefix, suffix, split)

// for(int i=start; i<=end; i+=step)
//      prefix ## i ## suffix if(i!=last step) ##split
#define CCDK_PP_RANGE_WRAP_STEP(prefix, start, end, step, suffix, split)          \
			CCDK_PP_RANGE_INNER_CALL(start, end, step, CCDK_PP_RANGE_WRAP_CONNECT, prefix, suffix, split)
