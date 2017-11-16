#pragma once


#include<core/preprocessor/impl/byte_less.h>
#include<core/preprocessor/impl/byte_equal.h>
#include<core/preprocessor/impl/byte_shift_attribute.h>
#include<core/preprocessor/impl/byte_shift_right.h>
#include<core/preprocessor/impl/byte_sub.h>
#include<core/preprocessor/impl/byte_add.h>
#include<core/preprocessor/impl/byte_shift_left.h>
#include<core/preprocessor/impl/uint8_to_byte.h>
#include<core/preprocessor/impl/uint8_to_bool.h>

#include<core/preprocessor/bool_not.h>
#include<core/preprocessor/uint8_sub_n.h>

//    refer to CCDK_PP_DIV
//helper function for CCDK_PP_BYTE_SHIFT_ATTR expand
#define CCDK_PP_BYTE_MOD_CONNECT_REPARSE1(x, y) CCDK_PP_BYTE_MOD_CONNECT_REPARSE1_STEP1(x, y)
#define CCDK_PP_BYTE_MOD_CONNECT_REPARSE1_STEP1(x, y) CCDK_PP_BYTE_MOD_CONNECT_REPARSE1_STEP2(x y)
#define CCDK_PP_BYTE_MOD_CONNECT_REPARSE1_STEP2(result) result
#define CCDK_PP_BYTE_MOD_CONNECT_REPARSE2(x, y) CCDK_PP_BYTE_MOD_CONNECT_REPARSE2_STEP1(x, y)
#define CCDK_PP_BYTE_MOD_CONNECT_REPARSE2_STEP1(x, y) CCDK_PP_BYTE_MOD_CONNECT_REPARSE2_STEP2(x y)
#define CCDK_PP_BYTE_MOD_CONNECT_REPARSE2_STEP2(result) result

#define CCDK_PP_BYTE_MOD(byte1, byte2)                               \
			CCDK_PP_BYTE_MOD_CONNECT_REPARSE1( CCDK_PP_IF(            \
				CCDK_PP_BYTE_LESS(byte2, byte1),                     \
				CCDK_PP_BYTE_MOD_LT,                                 \
				CCDK_PP_BYTE_MOD_GE),                                \
				(byte1, byte2, CCDK_PP_BYTE_SHIFT_ATTR(byte2)))

//byte2 >= byte1
#define CCDK_PP_BYTE_MOD_GE(byte1, byte2, _0,_1,_2,_3 )              \
			CCDK_PP_IF(CCDK_PP_BYTE_EQUAL(byte1, byte2),              \
				CCDK_PP_BYTE_MOD_EQ,                                 \
				CCDK_PP_BYTE_MOD_GT)(byte1)

//byte1 <  byte2 no change
#define CCDK_PP_BYTE_MOD_GT(byte) byte
//byte1 == byte2 return 0
#define CCDK_PP_BYTE_MOD_EQ(byte) (0,0,0,0,0,0,0,0)  
//return result
#define CCDK_PP_BYTE_MOD_RETURN(byte, ...)  byte

//byte1 >  byte2 
#define CCDK_PP_BYTE_MOD_LT(byte1, byte2, can_shift, lzeros, rzeros, bit_width) \
			CCDK_PP_BYTE_MOD_CONNECT_REPARSE2(                                  \
				CCDK_PP_BYTE_MOD_DO_1TH_SHIFT,                                  \
				(byte1, byte2, CCDK_PP_BYTE_SHIFT_ATTR( byte1), lzeros, rzeros) )


// -------------------------------  1th shift,cmp,sub ----------------------------------------
//do 1th shift
#define CCDK_PP_BYTE_MOD_DO_1TH_SHIFT(byte1, byte2, _0, lzeros1, _1,_2, lzeros2, _3)        \
			CCDK_PP_BYTE_MOD_DO_1TH_SHIFT_HELPER(byte1, byte2, CCDK_PP_SUB(lzeros2, lzeros1))

#define CCDK_PP_BYTE_MOD_DO_1TH_SHIFT_HELPER(byte1, byte2, sft_cnt)                         \
			CCDK_PP_BYTE_MOD_DO_1TH_CMP(CCDK_PP_BYTE_SHIFT_LEFT(byte2, sft_cnt),             \
					byte1, byte2, sft_cnt)
//do 1th cmp 
//byte2_shift <= byte1 do (byte1 - byte2_shift) 
#define CCDK_PP_BYTE_MOD_DO_1TH_CMP(byte2_shift, byte1, byte2, sft_cnt )                    \
			CCDK_PP_IF(CCDK_PP_BYTE_LESS(byte1, byte2_shift),                                \
				CCDK_PP_BYTE_MOD_DO_2TH_SHIFT,                                              \
				CCDK_PP_BYTE_MOD_DO_1TH_SUB)(                                               \
					byte2_shift, byte1, byte2, CCDK_PP_SUB1(sft_cnt)) 

// if need , do 1th sub
#define CCDK_PP_BYTE_MOD_DO_1TH_SUB(byte2_shift, byte1, byte2, sft_cnt)                     \
			CCDK_PP_BYTE_MOD_DO_2TH_SHIFT(byte2_shift, CCDK_PP_BYTE_SUB(byte1,               \
				byte2_shift),byte2, sft_cnt)                   

//----------------------------------------- 2th shift, cmp, sub ------------------------------------
#define CCDK_PP_BYTE_MOD_DO_2TH_SHIFT( byte2_shift, byte1, byte2, sft_cnt)	               \
			CCDK_PP_BYTE_MOD_DO_2TH_CMP(CCDK_PP_BYTE_SHIFT_LEFT(byte2, sft_cnt),             \
				byte1, byte2, sft_cnt)

#define CCDK_PP_BYTE_MOD_DO_2TH_CMP(byte2_shift, byte1, byte2, sft_cnt)                     \
			CCDK_PP_BYTE_MOD_DO_2TH_CMP_HELPER(CCDK_PP_BYTE_LESS(byte1, byte2_shift),        \
				byte2_shift, byte1, byte2, sft_cnt)

#define CCDK_PP_BYTE_MOD_DO_2TH_CMP_HELPER(cmp, byte2_shift, byte1, byte2, sft_cnt)         \
			CCDK_PP_IF(cmp,                                                                 \
				CCDK_PP_BYTE_MOD_DO_2TH_CHECK_END,                                          \
				CCDK_PP_BYTE_MOD_DO_2TH_SUB)( byte2_shift, byte1, byte2, sft_cnt)

#define CCDK_PP_BYTE_MOD_DO_2TH_SUB(byte2_shift, byte1, byte2, sft_cnt)                     \
			CCDK_PP_BYTE_MOD_DO_2TH_CHECK_END(byte2_shift,                                  \
				CCDK_PP_BYTE_SUB(byte1, byte2_shift), byte2, sft_cnt)

//if sft_cnt == 0 return result
#define CCDK_PP_BYTE_MOD_DO_2TH_CHECK_END(_, byte1, byte2, sft_cnt)                         \
			CCDK_PP_IF( CCDK_PP_INT_BOOL(sft_cnt),                                           \
				CCDK_PP_BYTE_MOD_DO_3TH_SHIFT,                                              \
				CCDK_PP_BYTE_MOD_RETURN)(byte1, byte2, CCDK_PP_SUB1(sft_cnt))

//---------------------------------------- 3th shift, cmp, sub --------------------------------
#define CCDK_PP_BYTE_MOD_DO_3TH_SHIFT(byte1, byte2, sft_cnt)	                               \
			CCDK_PP_BYTE_MOD_DO_3TH_CMP(CCDK_PP_BYTE_SHIFT_LEFT(byte2, sft_cnt),             \
				byte1, byte2, sft_cnt)

#define CCDK_PP_BYTE_MOD_DO_3TH_CMP(byte2_shift, byte1, byte2, sft_cnt)                     \
			CCDK_PP_BYTE_MOD_DO_3TH_CMP_HELPER(CCDK_PP_BYTE_LESS(byte1, byte2_shift),        \
				byte2_shift, byte1, byte2, sft_cnt)

#define CCDK_PP_BYTE_MOD_DO_3TH_CMP_HELPER(cmp, byte2_shift, byte1, byte2, sft_cnt)         \
			CCDK_PP_IF(cmp,                                                                 \
				CCDK_PP_BYTE_MOD_DO_3TH_CHECK_END,                                          \
				CCDK_PP_BYTE_MOD_DO_3TH_SUB)( byte2_shift, byte1, byte2, sft_cnt)

#define CCDK_PP_BYTE_MOD_DO_3TH_SUB( byte2_shift, byte1, byte2, sft_cnt)                    \
			CCDK_PP_BYTE_MOD_DO_3TH_CHECK_END(byte2_shift,                                  \
				CCDK_PP_BYTE_SUB(byte1, byte2_shift), byte2, sft_cnt)

//if sft_cnt == 0 return result
#define CCDK_PP_BYTE_MOD_DO_3TH_CHECK_END(_, byte1, byte2, sft_cnt)                         \
			CCDK_PP_IF( CCDK_PP_INT_BOOL(sft_cnt),                                           \
				CCDK_PP_BYTE_MOD_DO_4TH_SHIFT,                                              \
				CCDK_PP_BYTE_MOD_RETURN)(byte1, byte2, CCDK_PP_SUB1(sft_cnt))

//---------------------------------------- 4th shift, cmp, sub --------------------------------
#define CCDK_PP_BYTE_MOD_DO_4TH_SHIFT(byte1, byte2, sft_cnt)	                               \
			CCDK_PP_BYTE_MOD_DO_4TH_CMP(CCDK_PP_BYTE_SHIFT_LEFT(byte2, sft_cnt),             \
				byte1, byte2, sft_cnt)

#define CCDK_PP_BYTE_MOD_DO_4TH_CMP(byte2_shift, byte1, byte2, sft_cnt)                     \
			CCDK_PP_BYTE_MOD_DO_4TH_CMP_HELPER(CCDK_PP_BYTE_LESS(byte1, byte2_shift),        \
				byte2_shift, byte1, byte2, sft_cnt)

#define CCDK_PP_BYTE_MOD_DO_4TH_CMP_HELPER(cmp, byte2_shift, byte1, byte2, sft_cnt)         \
			CCDK_PP_IF(cmp,                                                                 \
				CCDK_PP_BYTE_MOD_DO_4TH_CHECK_END,                                          \
				CCDK_PP_BYTE_MOD_DO_4TH_SUB)( byte2_shift, byte1, byte2, sft_cnt)

#define CCDK_PP_BYTE_MOD_DO_4TH_SUB(byte2_shift, byte1, byte2, sft_cnt)                     \
			CCDK_PP_BYTE_MOD_DO_4TH_CHECK_END(byte2_shift,                                  \
				CCDK_PP_BYTE_SUB(byte1, byte2_shift), byte2, sft_cnt)

//if sft_cnt == 0 return result
#define CCDK_PP_BYTE_MOD_DO_4TH_CHECK_END(_, byte1, byte2, sft_cnt)                         \
			CCDK_PP_IF( CCDK_PP_INT_BOOL(sft_cnt),                                           \
				CCDK_PP_BYTE_MOD_DO_5TH_SHIFT,                                              \
				CCDK_PP_BYTE_MOD_RETURN)(byte1, byte2, CCDK_PP_SUB1(sft_cnt))
//---------------------------------------- 5th shift, cmp, sub --------------------------------
#define CCDK_PP_BYTE_MOD_DO_5TH_SHIFT(byte1, byte2, sft_cnt)	             	               \
			CCDK_PP_BYTE_MOD_DO_5TH_CMP(CCDK_PP_BYTE_SHIFT_LEFT(byte2, sft_cnt),             \
				byte1, byte2, sft_cnt)

#define CCDK_PP_BYTE_MOD_DO_5TH_CMP(byte2_shift, byte1, byte2, sft_cnt)                     \
			CCDK_PP_BYTE_MOD_DO_5TH_CMP_HELPER(CCDK_PP_BYTE_LESS(byte1, byte2_shift),        \
				byte2_shift, byte1, byte2, sft_cnt)

#define CCDK_PP_BYTE_MOD_DO_5TH_CMP_HELPER(cmp, byte2_shift, byte1, byte2, sft_cnt)         \
			CCDK_PP_IF(cmp,                                                                 \
				CCDK_PP_BYTE_MOD_DO_5TH_CHECK_END,                                          \
				CCDK_PP_BYTE_MOD_DO_5TH_SUB)( byte2_shift, byte1, byte2, sft_cnt)

#define CCDK_PP_BYTE_MOD_DO_5TH_SUB(byte2_shift, byte1, byte2, sft_cnt)                     \
			CCDK_PP_BYTE_MOD_DO_5TH_CHECK_END(byte2_shift,                                  \
				CCDK_PP_BYTE_SUB(byte1, byte2_shift), byte2, sft_cnt)

//if sft_cnt == 0 return result
#define CCDK_PP_BYTE_MOD_DO_5TH_CHECK_END(_, byte1, byte2, sft_cnt)                         \
			CCDK_PP_IF( CCDK_PP_INT_BOOL(sft_cnt),                                           \
				CCDK_PP_BYTE_MOD_DO_6TH_SHIFT,                                              \
				CCDK_PP_BYTE_MOD_RETURN)(byte1, byte2, CCDK_PP_SUB1(sft_cnt))
//---------------------------------------- 6th shift, cmp, sub --------------------------------
#define CCDK_PP_BYTE_MOD_DO_6TH_SHIFT(byte1, byte2, sft_cnt)	             	               \
			CCDK_PP_BYTE_MOD_DO_6TH_CMP(CCDK_PP_BYTE_SHIFT_LEFT(byte2, sft_cnt),             \
				byte1, byte2, sft_cnt)

#define CCDK_PP_BYTE_MOD_DO_6TH_CMP(byte2_shift, byte1, byte2, sft_cnt)                     \
			CCDK_PP_BYTE_MOD_DO_6TH_CMP_HELPER(CCDK_PP_BYTE_LESS(byte1, byte2_shift),        \
				byte2_shift, byte1, byte2, sft_cnt)

#define CCDK_PP_BYTE_MOD_DO_6TH_CMP_HELPER(cmp, byte2_shift, byte1, byte2, sft_cnt)         \
			CCDK_PP_IF(cmp,                                                                 \
				CCDK_PP_BYTE_MOD_DO_6TH_CHECK_END,                                          \
				CCDK_PP_BYTE_MOD_DO_6TH_SUB)( byte2_shift, byte1, byte2, sft_cnt)

#define CCDK_PP_BYTE_MOD_DO_6TH_SUB(byte2_shift, byte1, byte2, sft_cnt)                     \
			CCDK_PP_BYTE_MOD_DO_6TH_CHECK_END(byte2_shift,                                  \
				CCDK_PP_BYTE_SUB(byte1, byte2_shift), byte2, sft_cnt)

//if sft_cnt == 0 return result
#define CCDK_PP_BYTE_MOD_DO_6TH_CHECK_END(_, byte1, byte2, sft_cnt)                         \
			CCDK_PP_IF( CCDK_PP_INT_BOOL(sft_cnt),                                           \
				CCDK_PP_BYTE_MOD_DO_7TH_SHIFT,                                              \
				CCDK_PP_BYTE_MOD_RETURN)(byte1, byte2, CCDK_PP_SUB1(sft_cnt))

//---------------------------------------- 7th shift, cmp, sub --------------------------------
#define CCDK_PP_BYTE_MOD_DO_7TH_SHIFT(byte1, byte2, sft_cnt)	             	               \
			CCDK_PP_BYTE_MOD_DO_7TH_CMP(CCDK_PP_BYTE_SHIFT_LEFT(byte2, sft_cnt),             \
				byte1, byte2, sft_cnt)

#define CCDK_PP_BYTE_MOD_DO_7TH_CMP(byte2_shift, byte1, byte2, sft_cnt)                     \
			CCDK_PP_BYTE_MOD_DO_7TH_CMP_HELPER(CCDK_PP_BYTE_LESS(byte1, byte2_shift),        \
				byte2_shift, byte1, byte2, sft_cnt)

//byte1 < byte2_shift  return result
#define CCDK_PP_BYTE_MOD_DO_7TH_CMP_HELPER(cmp, byte2_shift, byte1, byte2, sft_cnt)         \
			CCDK_PP_IF(cmp,                                                                 \
				CCDK_PP_BYTE_MOD_RETURN,                                                    \
				CCDK_PP_BYTE_MOD_DO_7TH_SUB)( byte2_shift, byte1, byte2, sft_cnt)

#define CCDK_PP_BYTE_MOD_DO_7TH_SUB(byte2_shift, byte1, byte2, sft_cnt)                     \
			CCDK_PP_BYTE_MOD_RETURN(CCDK_PP_BYTE_SUB(byte1, byte2_shift), byte2, sft_cnt)   
////////////////////////////////////////////////////////////////////////////////////////////////