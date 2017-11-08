#pragma once

#include<core/preprocessor/impl/bit4x4_sub.h>
#include<core/preprocessor/impl/bit4_all_zero.h>
#include<core/preprocessor/bool_if.h>
#include<core/preprocessor/seq_unpack.h>

// byte1 - byte2
// do unpack byte1 and byte2
#define A3D_PP_BYTE_SUB(byte1, byte2)                         \
			A3D_PP_BYTE_SUB_CONNECT1( A3D_PP_BYTE_SUB_STEP1,  \
				(A3D_PP_UNPACK(byte1),            \
					A3D_PP_UNPACK(byte2))) 

//do high-4bit and low-4bit sub
#define A3D_PP_BYTE_SUB_STEP1(p7,p6,p5,p4,p3,p2,p1,p0, q7,q6,q5,q4,q3,q2,q1,q0)  \
			A3D_PP_BYTE_SUB_CONNECT2(A3D_PP_BYTE_SUB_STEP2,                      \
				(A3D_PP_BIT4x4_SUB(p7,p6,p5,p4,q7,q6,q5,q4),                     \
					A3D_PP_BIT4x4_SUB(p3, p2, p1, p0, q3, q2, q1, q0))) 

// if(s1==1 || £¨s0==1 && high-4bit ==0000) )
//    overflow return 0
// else 
//    check borrow bit 
#define A3D_PP_BYTE_SUB_STEP2(s1,r7,r6,r5,r4,s0,r3,r2,r1,r0)                                 \
			A3D_PP_IF(                                                                       \
				A3D_PP_OR(s1, A3D_PP_AND(s0, A3D_PP_BIT4_ALL_ZERO(r7, r6, r5, r4))),         \
				A3D_PP_BYTE_SUB_LT_ZERO,                                                     \
				A3D_PP_BYTE_SUB_GE_ZERO)(r7, r6, r5, r4, s0, r3, r2, r1, r0)
//overflow return
#define A3D_PP_BYTE_SUB_LT_ZERO(...) (0,0,0,0,0,0,0,0)

//if( s0 == 1) 
//    result = (high-4bit - 1) + low-4bit  
//else 
//    result = high-4bit + low-4bit
#define A3D_PP_BYTE_SUB_GE_ZERO(r7, r6, r5, r4, s0, r3, r2, r1, r0)         \
			A3D_PP_IF(s0,                                                   \
				A3D_PP_BYTE_SUB_ON_SIGN,                                    \
				A3D_PP_BYTE_SUB_NO_CHANGE)(r7, r6, r5, r4, r3, r2, r1, r0)

//return high-4bit + low-4bit
#define A3D_PP_BYTE_SUB_NO_CHANGE(...) (__VA_ARGS__)

//return (high-4bit -1) + low-4bit  
#define A3D_PP_BYTE_SUB_ON_SIGN(r7, r6, r5, r4, r3, r2, r1, r0) (A3D_PP_BYTE_SUB_ONE_CONDI_VALUE##r7##r6##r5##r4,r3,r2,r1,r0)

// high-4bit -1 lookup table
#define A3D_PP_BYTE_SUB_ONE_CONDI_VALUE0001 0,0,0,0
#define A3D_PP_BYTE_SUB_ONE_CONDI_VALUE0010 0,0,0,1
#define A3D_PP_BYTE_SUB_ONE_CONDI_VALUE0011 0,0,1,0
#define A3D_PP_BYTE_SUB_ONE_CONDI_VALUE0100 0,0,1,1
#define A3D_PP_BYTE_SUB_ONE_CONDI_VALUE0101 0,1,0,0
#define A3D_PP_BYTE_SUB_ONE_CONDI_VALUE0110 0,1,0,1
#define A3D_PP_BYTE_SUB_ONE_CONDI_VALUE0111 0,1,1,0
#define A3D_PP_BYTE_SUB_ONE_CONDI_VALUE1000 0,1,1,1
#define A3D_PP_BYTE_SUB_ONE_CONDI_VALUE1001 1,0,0,0
#define A3D_PP_BYTE_SUB_ONE_CONDI_VALUE1010 1,0,0,1
#define A3D_PP_BYTE_SUB_ONE_CONDI_VALUE1011 1,0,1,0
#define A3D_PP_BYTE_SUB_ONE_CONDI_VALUE1100 1,0,1,1
#define A3D_PP_BYTE_SUB_ONE_CONDI_VALUE1101 1,1,0,0
#define A3D_PP_BYTE_SUB_ONE_CONDI_VALUE1110 1,1,0,1
#define A3D_PP_BYTE_SUB_ONE_CONDI_VALUE1111 1,1,1,0

//helper function
#define A3D_PP_BYTE_SUB_CONNECT1(x, y) A3D_PP_BYTE_SUB_CONNECT1_STEP1(x, y)
#define A3D_PP_BYTE_SUB_CONNECT1_STEP1(x, y) A3D_PP_BYTE_SUB_CONNECT1_STEP2(x y)
#define A3D_PP_BYTE_SUB_CONNECT1_STEP2(result) result
#define A3D_PP_BYTE_SUB_CONNECT2(x, y) A3D_PP_BYTE_SUB_CONNECT2_STEP1(x, y)
#define A3D_PP_BYTE_SUB_CONNECT2_STEP1(x, y) A3D_PP_BYTE_SUB_CONNECT2_STEP2(x y)
#define A3D_PP_BYTE_SUB_CONNECT2_STEP2(result) result
