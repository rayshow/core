#pragma once
#include<core/preprocessor/range_inner_call.h>
#include<core/preprocessor/seq_unpack.h>
#include<core/preprocessor/uint8_if.h>
#include<core/preprocessor/ingore.h>

#define A3D_PP_RANGE_PREFIX_CONNECT(i, count_down, prefix, split) \
			prefix ## i A3D_PP_INT_IF( count_down, A3D_PP_UNPACK, A3D_PP_INGORE)(split)

// for(int i=start; i<=end; ++i)
//      prefix ## i if(i!=end) split
#define  A3D_PP_RANGE_PREFIX(prefix, start, end, split)              \
			A3D_PP_RANGE_INNER_CALL(start, end, 1,                   \
				A3D_PP_RANGE_PREFIX_CONNECT, prefix, split)

// for(int i=0; i<=end; i+=step)
//      prefix ## i if(i!=end) ## split
#define A3D_PP_RANGE_PREFIX_STEP(prefix, start, end, step, split)    \
			A3D_PP_RANGE_INNER_CALL(start, end, step,                \
				A3D_PP_RANGE_PREFIX_CONNECT, prefix, split)
			