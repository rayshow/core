#pragma once
#include<core/preprocessor/range_inner_call.h>
#include<core/preprocessor/seq_unpack.h>
#include<core/preprocessor/uint8_if.h>
#include<core/preprocessor/ingore.h>

#define A3D_PP_RANGE_WRAP_CONNECT(i, count_down, prefix, suffix, split) \
			prefix ## i suffix A3D_PP_INT_IF( count_down, A3D_PP_UNPACK, A3D_PP_INGORE)(split)

// for(int i=start; i<=end; ++i)
//      prefix ## i ## suffix if(i!=end) ##split
#define  A3D_PP_RANGE_WRAP(prefix, start, end, suffix, split)                \
			A3D_PP_RANGE_INNER_CALL(start, end, 1,                           \
				A3D_PP_RANGE_WRAP_CONNECT, prefix, suffix, split)

// for(int i=start; i<=end; i+=step)
//      prefix ## i ## suffix if(i!=last step) ##split
#define A3D_PP_RANGE_WRAP_STEP(prefix, start, end, step, suffix, split)          \
			A3D_PP_RANGE_INNER_CALL(start, end, step, A3D_PP_RANGE_WRAP_CONNECT, prefix, suffix, split)
