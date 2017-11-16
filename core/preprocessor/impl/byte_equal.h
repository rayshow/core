#pragma once

#include<core/preprocessor/bool_if.h>
#include<core/preprocessor/seq_unpack.h>
#include<core/preprocessor/impl/bit4x4_equal.h>

#define CCDK_PP_BYTE_EQUAL(byte1, byte2)             \
			CCDK_PP_BYTE_EQUAL_CONNECT_REPARSE(      \
				CCDK_PP_BYTE_EQUAL_EXPLICIT,         \
					(CCDK_PP_UNPACK(byte1),      \
					 CCDK_PP_UNPACK(byte2)))

#define CCDK_PP_BYTE_EQUAL_EXPLICIT(x7,x6,x5,x4,x3,x2,x1,x0,y7,y6,y5,y4,y3,y2,y1,y0) \
			 CCDK_PP_IF( CCDK_PP_BIT4x4_EQUAL(x7,x6,x5,x4,y7,y6,y5,y4),               \
				CCDK_PP_BIT4x4_EQUAL,                                                \
                CCDK_PP_BIT4x4_EQUAL_FALSE)(x3,x2,x1,x0,y3,y2,y1,y0)

#define CCDK_PP_BYTE_EQUAL_CONNECT_REPARSE(x, y) CCDK_PP_BYTE_EQUAL_CONNECT_REPARSE_STEP1(x, y)
#define CCDK_PP_BYTE_EQUAL_CONNECT_REPARSE_STEP1(x, y) CCDK_PP_BYTE_EQUAL_CONNECT_REPARSE_STEP2(x y)
#define CCDK_PP_BYTE_EQUAL_CONNECT_REPARSE_STEP2(result) result
