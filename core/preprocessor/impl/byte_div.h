#pragma once

#include<core/preprocessor/bool_if.h>
#include<core/preprocessor/bool_not.h>
#include<core/preprocessor/impl/uint8_to_byte.h>
#include<core/preprocessor/impl/uint8_to_bool.h>
#include<core/preprocessor/impl/byte_less.h>
#include<core/preprocessor/impl/byte_equal.h>
#include<core/preprocessor/impl/byte_shift_attribute.h>
#include<core/preprocessor/impl/byte_shift_right.h>
#include<core/preprocessor/impl/byte_sub.h>
#include<core/preprocessor/impl/byte_add.h>
#include<core/preprocessor/impl/byte_shift_left.h>

//    byte1 divator    byte2 divend
//    sft_cnt = lzero2 - lzero1 = 5 - 0 = 5
//
//       0 0 0 1 1 0 0 1   
//     ___________________
//  110| 1 0 0 1 1 0 1 0   
//       1 1 0 0 0 0 0 0   cmp  shift_left(byte2,sft_cnt) > byte1 ==> 0, --sft_cnt (4)
//       ---------------
//      -0 1 1 0 0 0 0 0   cmp  shift_left(byte2,sft_cnt) < byte1 ==> 1, byte1 = byte1 - shift_left(byte2,sft_cnt), --sft_cnt (3)
//       0 0 1 1 1 0 1 0      
//      -0 0 1 1 0 0 0 0   cmp  shift_left(byte2,sft_cnt) < byte1 ==> 1, byte1 = byte1 - shift_left(byte2,sft_cnt), -- sft_cnt (2)
//       0 0 0 0 1 0 1 0   
//       0 0 0 1 1 0 0 0   cmp  shift_left(byte2,sft_cnt) > byte1 ==> 0, --sft_cnt (1)
//       0 0 0 0 1 1 0 0   cmp  shift_left(byte2,sft_cnt) > byte1 ==> 0, --sft_cnt (0)
//       0 0 0 0 0 1 1 0   cmp  shift_left(byte2,sft_cnt) < byte1 ==> 1, byte1 = byte1 - shift_left(byte2,sft_cnt), 
//       0 0 0 0 0 1 0 0   if   sft_cnt == 0 return result
//                 1 0 0   last byte1  is mod result
//  will at most need 7 time cmp to get result

//helper function
#define A3D_PP_BYTE_DIV_CONNECT_REPARSE1(x, y) A3D_PP_BYTE_DIV_CONNECT_REPARSE1_STEP1(x, y)
#define A3D_PP_BYTE_DIV_CONNECT_REPARSE1_STEP1(x, y) A3D_PP_BYTE_DIV_CONNECT_REPARSE1_STEP2(x y)
#define A3D_PP_BYTE_DIV_CONNECT_REPARSE1_STEP2(result) result
#define A3D_PP_BYTE_DIV_CONNECT_REPARSE2(x, y) A3D_PP_BYTE_DIV_CONNECT_REPARSE2_STEP1(x, y)
#define A3D_PP_BYTE_DIV_CONNECT_REPARSE2_STEP1(x, y) A3D_PP_BYTE_DIV_CONNECT_REPARSE2_STEP2(x y)
#define A3D_PP_BYTE_DIV_CONNECT_REPARSE2_STEP2(result) result

// byte1 / byte2
#define A3D_PP_BYTE_DIV(byte1, byte2)                                                   \
			A3D_PP_BYTE_DIV_CONNECT_REPARSE1(                                           \
				A3D_PP_IF( A3D_PP_BYTE_LESS(byte2, byte1),                              \
					A3D_PP_BYTE_DIV_LT,                                                 \
					A3D_PP_BYTE_DIV_GE),(byte1, byte2, A3D_PP_BYTE_SHIFT_ATTR(byte2)))

// byte2 greater or equal then byte1
#define A3D_PP_BYTE_DIV_GE(byte1, byte2, _0,_1,_2,_3 )                                  \
			A3D_PP_IF( A3D_PP_BYTE_EQUAL(byte1, byte2),                                 \
				A3D_PP_BYTE_DIV_EQ,                                                     \
				A3D_PP_BYTE_DIV_GT)

//byte1 <  byte2 return 0
#define A3D_PP_BYTE_DIV_GT (0,0,0,0,0,0,0,0)  
//byte1 == byte2 return 1
#define A3D_PP_BYTE_DIV_EQ (0,0,0,0,0,0,0,1)  
//byte1 >  byte2 
#define A3D_PP_BYTE_DIV_LT(byte1, byte2, can_shift, lzeros, rzeros, bit_width)  \
			A3D_PP_BYTE_DIV_CONNECT_REPARSE2(A3D_PP_IF( can_shift,              \
				A3D_PP_BYTE_DIV_DO_SHIFT_INSTEAD,                               \
				A3D_PP_BYTE_DIV_DO_1TH_SHIFT),                                  \
				(byte1, byte2, A3D_PP_BYTE_SHIFT_ATTR( byte1), lzeros, rzeros))

//if byte2 is 0,1,2,4,8,16,32,64,128 do shift instead of div
// div 0 will cause a div zero error
// div 1 just unchange
#define A3D_PP_BYTE_DIV_DO_SHIFT_INSTEAD(byte1, byte2, _0, _1, _2, _3,_4, rzeros2)  A3D_PP_BYTE_SHIFT_RIGHT(byte1, rzeros2)

//return result
#define A3D_PP_BYTE_DIV_RETURN(result, ...)  result

// -------------------------------  1th shift,cmp,sub ----------------------------------------
//do 1th shift
#define A3D_PP_BYTE_DIV_DO_1TH_SHIFT(byte1, byte2, _0, lzeros1, _1,_2, lzeros2, _3)    \
			A3D_PP_BYTE_DIV_DO_1TH_SHIFT_HELPER(byte1, byte2, A3D_PP_SUB(lzeros2, lzeros1))
		
#define A3D_PP_BYTE_DIV_DO_1TH_SHIFT_HELPER(byte1, byte2, sft_cnt)                         \
			A3D_PP_BYTE_DIV_DO_1TH_CMP(A3D_PP_BYTE_SHIFT_LEFT(byte2, sft_cnt),             \
					byte1, byte2, sft_cnt)

//do 1th cmp
#define A3D_PP_BYTE_DIV_DO_1TH_CMP(byte2_shift, byte1, byte2, sft_cnt )                    \
			A3D_PP_BYTE_DIV_DO_1TH_CMP_HELPER(A3D_PP_BYTE_LESS(byte1, byte2_shift),        \
				byte2_shift, byte1, byte2, sft_cnt )

//-- sft_cnt and generate 1th result
//           if   cmp = 1 =>bit = 0, do next shift
//           else cmp = 0 =>bit = 1, do 1th sub and byte1 = byte1 - byte2_shift 
#define A3D_PP_BYTE_DIV_DO_1TH_CMP_HELPER(cmp, byte2_shift, byte1, byte2, sft_cnt)         \
			A3D_PP_IF(cmp,                                                                 \
				A3D_PP_BYTE_DIV_DO_2TH_SHIFT,                                              \
				A3D_PP_BYTE_DIV_DO_1TH_SUB)(                                               \
					A3D_PP_BYTE_SHIFT_LEFT( A3D_PP_INT_BYTE( A3D_PP_NOT(cmp)), sft_cnt),   \
					byte2_shift, byte1, byte2, A3D_PP_SUB1(sft_cnt)  ) 

// if need , do 1th sub
#define A3D_PP_BYTE_DIV_DO_1TH_SUB(result, byte2_shift, byte1, byte2, sft_cnt)             \
			A3D_PP_BYTE_DIV_DO_2TH_SHIFT(result,byte2_shift,                               \
				A3D_PP_BYTE_SUB(byte1, byte2_shift),byte2, sft_cnt)                   

//----------------------------------------- 2th shift, cmp, sub ------------------------------------
#define A3D_PP_BYTE_DIV_DO_2TH_SHIFT(result, _0, byte1, byte2, sft_cnt)	             	   \
			A3D_PP_BYTE_DIV_DO_2TH_CMP(result, A3D_PP_BYTE_SHIFT_LEFT(byte2, sft_cnt),     \
				byte1, byte2, sft_cnt)

#define A3D_PP_BYTE_DIV_DO_2TH_CMP(result, byte2_shift, byte1, byte2, sft_cnt)             \
			A3D_PP_BYTE_DIV_DO_2TH_CMP_HELPER(result, A3D_PP_BYTE_LESS(byte1, byte2_shift),\
				byte2_shift, byte1, byte2, sft_cnt)

#define A3D_PP_BYTE_DIV_DO_2TH_CMP_HELPER(result, cmp, byte2_shift, byte1, byte2, sft_cnt) \
			A3D_PP_IF(cmp,                                                                 \
				A3D_PP_BYTE_DIV_DO_2TH_CHECK_END,                                          \
				A3D_PP_BYTE_DIV_DO_2TH_SUB)(                                               \
					A3D_PP_BYTE_ADD(result, A3D_PP_BYTE_SHIFT_LEFT( A3D_PP_INT_BYTE(       \
					A3D_PP_NOT(cmp)),sft_cnt) ), byte2_shift, byte1, byte2, sft_cnt)  

#define A3D_PP_BYTE_DIV_DO_2TH_SUB(result, byte2_shift, byte1, byte2, sft_cnt)             \
			A3D_PP_BYTE_DIV_DO_2TH_CHECK_END(result, byte2_shift,                          \
				A3D_PP_BYTE_SUB(byte1, byte2_shift), byte2, sft_cnt)

//if sft_cnt == 0 return result
#define A3D_PP_BYTE_DIV_DO_2TH_CHECK_END(result, _0, byte1, byte2, sft_cnt)                \
			A3D_PP_IF( A3D_PP_INT_BOOL(sft_cnt),                                           \
				A3D_PP_BYTE_DIV_DO_3TH_SHIFT,                                              \
				A3D_PP_BYTE_DIV_RETURN)(result, byte1, byte2, A3D_PP_SUB1(sft_cnt))
//---------------------------------------- 3th shift, cmp, sub --------------------------------
#define A3D_PP_BYTE_DIV_DO_3TH_SHIFT(result, byte1, byte2, sft_cnt)	                       \
			A3D_PP_BYTE_DIV_DO_3TH_CMP(result, A3D_PP_BYTE_SHIFT_LEFT(byte2, sft_cnt),     \
				byte1, byte2, sft_cnt)

#define A3D_PP_BYTE_DIV_DO_3TH_CMP(result, byte2_shift, byte1, byte2, sft_cnt)             \
			A3D_PP_BYTE_DIV_DO_3TH_CMP_HELPER(result, A3D_PP_BYTE_LESS(byte1, byte2_shift),\
				byte2_shift, byte1, byte2, sft_cnt)

#define A3D_PP_BYTE_DIV_DO_3TH_CMP_HELPER(result, cmp, byte2_shift, byte1, byte2, sft_cnt) \
			A3D_PP_IF(cmp,                                                                 \
				A3D_PP_BYTE_DIV_DO_3TH_CHECK_END,                                          \
				A3D_PP_BYTE_DIV_DO_3TH_SUB)(                                               \
				    A3D_PP_BYTE_ADD(result, A3D_PP_BYTE_SHIFT_LEFT( A3D_PP_INT_BYTE(       \
					A3D_PP_NOT(cmp)),sft_cnt) ), byte2_shift, byte1, byte2, sft_cnt)

#define A3D_PP_BYTE_DIV_DO_3TH_SUB(result, byte2_shift, byte1, byte2, sft_cnt)             \
			A3D_PP_BYTE_DIV_DO_3TH_CHECK_END(result, byte2_shift,                          \
				A3D_PP_BYTE_SUB(byte1, byte2_shift), byte2, sft_cnt)

//if sft_cnt == 0 return result
#define A3D_PP_BYTE_DIV_DO_3TH_CHECK_END(result, _0, byte1, byte2, sft_cnt)                \
			A3D_PP_IF( A3D_PP_INT_BOOL(sft_cnt),                                           \
				A3D_PP_BYTE_DIV_DO_4TH_SHIFT,                                              \
				A3D_PP_BYTE_DIV_RETURN)(result, byte1, byte2, A3D_PP_SUB1(sft_cnt))

//---------------------------------------- 4th shift, cmp, sub --------------------------------
#define A3D_PP_BYTE_DIV_DO_4TH_SHIFT(result, byte1, byte2, sft_cnt)	                       \
			A3D_PP_BYTE_DIV_DO_4TH_CMP(result, A3D_PP_BYTE_SHIFT_LEFT(byte2, sft_cnt),     \
				byte1, byte2, sft_cnt)

#define A3D_PP_BYTE_DIV_DO_4TH_CMP(result, byte2_shift, byte1, byte2, sft_cnt)             \
			A3D_PP_BYTE_DIV_DO_4TH_CMP_HELPER(result, A3D_PP_BYTE_LESS(byte1, byte2_shift),\
				byte2_shift, byte1, byte2, sft_cnt)

#define A3D_PP_BYTE_DIV_DO_4TH_CMP_HELPER(result, cmp, byte2_shift, byte1, byte2, sft_cnt) \
			A3D_PP_IF(cmp,                                                                 \
				A3D_PP_BYTE_DIV_DO_4TH_CHECK_END,                                          \
				A3D_PP_BYTE_DIV_DO_4TH_SUB)(                                               \
					A3D_PP_BYTE_ADD(result, A3D_PP_BYTE_SHIFT_LEFT( A3D_PP_INT_BYTE(       \
					A3D_PP_NOT(cmp)),sft_cnt) ), byte2_shift, byte1, byte2, sft_cnt)

#define A3D_PP_BYTE_DIV_DO_4TH_SUB(result, byte2_shift, byte1, byte2, sft_cnt)             \
			A3D_PP_BYTE_DIV_DO_4TH_CHECK_END(result, byte2_shift,                          \
				A3D_PP_BYTE_SUB(byte1, byte2_shift), byte2, sft_cnt)

//if sft_cnt == 0 return result
#define A3D_PP_BYTE_DIV_DO_4TH_CHECK_END(result, _0, byte1, byte2, sft_cnt)                \
			A3D_PP_IF( A3D_PP_INT_BOOL(sft_cnt),                                           \
				A3D_PP_BYTE_DIV_DO_5TH_SHIFT,                                              \
				A3D_PP_BYTE_DIV_RETURN)(result, byte1, byte2, A3D_PP_SUB1(sft_cnt))
//---------------------------------------- 5th shift, cmp, sub --------------------------------
#define A3D_PP_BYTE_DIV_DO_5TH_SHIFT(result, byte1, byte2, sft_cnt)	             	       \
			A3D_PP_BYTE_DIV_DO_5TH_CMP(result, A3D_PP_BYTE_SHIFT_LEFT(byte2, sft_cnt),     \
				byte1, byte2, sft_cnt)

#define A3D_PP_BYTE_DIV_DO_5TH_CMP(result, byte2_shift, byte1, byte2, sft_cnt)             \
			A3D_PP_BYTE_DIV_DO_5TH_CMP_HELPER(result, A3D_PP_BYTE_LESS(byte1, byte2_shift),\
				byte2_shift, byte1, byte2, sft_cnt)

#define A3D_PP_BYTE_DIV_DO_5TH_CMP_HELPER(result, cmp, byte2_shift, byte1, byte2, sft_cnt) \
			A3D_PP_IF(cmp,                                                                 \
				A3D_PP_BYTE_DIV_DO_5TH_CHECK_END,                                          \
				A3D_PP_BYTE_DIV_DO_5TH_SUB)(                                               \
					A3D_PP_BYTE_ADD(result, A3D_PP_BYTE_SHIFT_LEFT( A3D_PP_INT_BYTE(       \
					A3D_PP_NOT(cmp)),sft_cnt) ), byte2_shift, byte1, byte2, sft_cnt)

#define A3D_PP_BYTE_DIV_DO_5TH_SUB(result, byte2_shift, byte1, byte2, sft_cnt)             \
			A3D_PP_BYTE_DIV_DO_5TH_CHECK_END(result, byte2_shift,                          \
				A3D_PP_BYTE_SUB(byte1, byte2_shift), byte2, sft_cnt)

//if sft_cnt == 0 return result
#define A3D_PP_BYTE_DIV_DO_5TH_CHECK_END(result, _0, byte1, byte2, sft_cnt)                \
			A3D_PP_IF( A3D_PP_INT_BOOL(sft_cnt),                                           \
				A3D_PP_BYTE_DIV_DO_6TH_SHIFT,                                              \
				A3D_PP_BYTE_DIV_RETURN)(result, byte1, byte2, A3D_PP_SUB1(sft_cnt))
//---------------------------------------- 6th shift, cmp, sub --------------------------------
#define A3D_PP_BYTE_DIV_DO_6TH_SHIFT(result, byte1, byte2, sft_cnt)	             	       \
			A3D_PP_BYTE_DIV_DO_6TH_CMP(result, A3D_PP_BYTE_SHIFT_LEFT(byte2, sft_cnt),     \
				byte1, byte2, sft_cnt)

#define A3D_PP_BYTE_DIV_DO_6TH_CMP(result, byte2_shift, byte1, byte2, sft_cnt)             \
			A3D_PP_BYTE_DIV_DO_6TH_CMP_HELPER(result, A3D_PP_BYTE_LESS(byte1, byte2_shift),\
				byte2_shift, byte1, byte2, sft_cnt)

#define A3D_PP_BYTE_DIV_DO_6TH_CMP_HELPER(result, cmp, byte2_shift, byte1, byte2, sft_cnt) \
			A3D_PP_IF(cmp,                                                                 \
				A3D_PP_BYTE_DIV_DO_6TH_CHECK_END,                                          \
				A3D_PP_BYTE_DIV_DO_6TH_SUB)(                                               \
					A3D_PP_BYTE_ADD(result, A3D_PP_BYTE_SHIFT_LEFT( A3D_PP_INT_BYTE(       \
					A3D_PP_NOT(cmp)),sft_cnt) ), byte2_shift, byte1, byte2, sft_cnt)

#define A3D_PP_BYTE_DIV_DO_6TH_SUB(result, byte2_shift, byte1, byte2, sft_cnt)             \
			A3D_PP_BYTE_DIV_DO_6TH_CHECK_END(result, byte2_shift,                          \
				A3D_PP_BYTE_SUB(byte1, byte2_shift), byte2, sft_cnt)

//if sft_cnt == 0 return result
#define A3D_PP_BYTE_DIV_DO_6TH_CHECK_END(result, _0, byte1, byte2, sft_cnt)                \
			A3D_PP_IF( A3D_PP_INT_BOOL(sft_cnt),                                           \
				A3D_PP_BYTE_DIV_DO_7TH_SHIFT,                                              \
				A3D_PP_BYTE_DIV_RETURN)(result, byte1, byte2, A3D_PP_SUB1(sft_cnt))

//---------------------------------------- 7th shift, cmp, sub --------------------------------
#define A3D_PP_BYTE_DIV_DO_7TH_SHIFT(result, byte1, byte2, sft_cnt)	             	       \
			A3D_PP_BYTE_DIV_DO_7TH_CMP(result, A3D_PP_BYTE_SHIFT_LEFT(byte2, sft_cnt),     \
				byte1, byte2, sft_cnt)

#define A3D_PP_BYTE_DIV_DO_7TH_CMP(result, byte2_shift, byte1, byte2, sft_cnt)             \
			A3D_PP_BYTE_DIV_DO_7TH_CMP_HELPER(result, A3D_PP_BYTE_LESS(byte1, byte2_shift),\
				byte2_shift, byte1, byte2, sft_cnt)

//
#define A3D_PP_BYTE_DIV_DO_7TH_CMP_HELPER(result, cmp, byte2_shift, byte1, byte2, sft_cnt) \
			A3D_PP_IF(cmp,                                                                 \
				A3D_PP_BYTE_DIV_RETURN,                                                    \
				A3D_PP_BYTE_DIV_DO_7TH_SUB)(                                               \
					A3D_PP_BYTE_ADD(result, A3D_PP_BYTE_SHIFT_LEFT(A3D_PP_INT_BYTE(        \
					A3D_PP_NOT(cmp)),sft_cnt) ), byte2_shift, byte1, byte2, sft_cnt)

#define A3D_PP_BYTE_DIV_DO_7TH_SUB(result, byte2_shift, byte1, byte2, sft_cnt)             \
			A3D_PP_BYTE_DIV_RETURN(result, A3D_PP_BYTE_SUB(byte1, byte2_shift)             \
				byte2, sft_cnt)
//-------------------------------------------------------------------------------------------

