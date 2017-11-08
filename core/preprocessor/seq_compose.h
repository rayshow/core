#pragma once
#include<core/preprocessor/impl/uint8_to_bool.h>
#include<core/preprocessor/seq_size.h>
#include<core/preprocessor/seq_at.h>
#include<core/preprocessor/while.h>
#include<core/preprocessor/uint8_if.h>
#include<core/preprocessor/uint8_comma_if.h>
#include<core/preprocessor/uint8_sub_one.h>
#include<core/preprocessor/bool_if_comma.h>
#include<core/preprocessor/uint8_mul.h>
#include<core/preprocessor/seq_expand.h>


// BUG  msvc15 A3D_PP_COMPOSE  pairs can't be (null, &) (null, &&) or (null, null), or the result will cause comma lose(don't know why)
// example: A3D_PP_COMPOSE( (const, volatile, const volatile, A3D_PP_NULL), (&, &&, A3D_PP_NULL ))
// use      A3D_PP_COMPOSE_EX( (const, volatile, const volatile), (&,&&,A3D_PP_NULL), A3D_PP_NULL, &, &&) instead


//generate seq compose of seq1 and seq2 
//(a1. a2) + (b1, b2) -> ( a1 b1, a1 b2, a2 b1, a2 b2)
#define A3D_PP_COMPOSE(seq1, seq2) A3D_PP_COMPOSE_HELPER(seq1, seq2, A3D_PP_SIZE(seq1), A3D_PP_SUB1( A3D_PP_SIZE(seq2)))
#define A3D_PP_COMPOSE_HELPER(seq1, seq2, size1, size2)     (A3D_PP_WHILE(A3D_PP_COMPOSE_CONDI, A3D_PP_COMPOSE_UPDATE,A3D_PP_COMPOSE_REPEAT, seq1, seq2, size2, size1, size2))


//generate seq compose of seq1 and seq2 + external items
#define A3D_PP_COMPOSE_EX(seq1, seq2,...)  A3D_PP_COMPOSE_EX_HELPER(seq1, seq2, A3D_PP_SIZE(seq1), A3D_PP_SUB1( A3D_PP_SIZE(seq2)), __VA_ARGS__)
#define A3D_PP_COMPOSE_EX_HELPER(seq1, seq2, size1, size2, ...) A3D_PP_EXPAND(  (A3D_PP_WHILE(A3D_PP_COMPOSE_CONDI, A3D_PP_COMPOSE_UPDATE,A3D_PP_COMPOSE_REPEAT, seq1, seq2, size2, size1, size2)), __VA_ARGS__ )

#define REPARSE_1(...) __VA_ARGS__

#define A3D_PP_COMPOSE3(seq1, seq2, seq3) A3D_PP_COMPOSE3_HELPER(seq1, seq2, seq3, A3D_PP_SIZE(seq1), A3D_PP_SUB1(A3D_PP_SIZE(seq2)), A3D_PP_SUB1(A3D_PP_SIZE(seq3)) )
#define A3D_PP_COMPOSE3_HELPER(seq1, seq2, seq3, size1, size2, size3) (A3D_PP_WHILE(A3D_PP_COMPOSE3_CONDI, A3D_PP_COMPOSE3_UPDATE,A3D_PP_COMPOSE3_REPEAT, seq1, seq2, seq3, size2, size3, size1, size2, size3))

#define A3D_PP_COMPOSE3_EX(seq1, seq2, seq3,...)   A3D_PP_COMPOSE3_EX_HELPER(seq1, seq2, seq3, A3D_PP_SIZE(seq1), A3D_PP_SUB1(A3D_PP_SIZE(seq2)), A3D_PP_SUB1(A3D_PP_SIZE(seq3)), __VA_ARGS__)
#define A3D_PP_COMPOSE3_EX_HELPER(seq1, seq2, seq3, size1, size2, size3,...) (__VA_ARGS__, A3D_PP_WHILE(A3D_PP_COMPOSE3_CONDI, A3D_PP_COMPOSE3_UPDATE,A3D_PP_COMPOSE3_REPEAT, seq1, seq2, seq3, size2, size3, size1, size2, size3))

// if count1 == 0 return;
#define A3D_PP_COMPOSE_CONDI(depth, _0, _1, _2, count1, _3)  A3D_PP_INT_BOOL(count1) 

// if   count2 == 0 ==> --count1, count2 = size2
// else             ==> --count2
#define A3D_PP_COMPOSE_UPDATE(depth, seq1, seq2, size2, count1, count2) \
			seq1, seq2, size2,                                          \
			A3D_PP_INT_IF(count2, count1, A3D_PP_SUB1(count1)),         \
			A3D_PP_INT_IF(count2, A3D_PP_SUB1(count2), size2)

//seq1[0] seq2[0],..., seq1[n-1] seq2[n-1]
#define A3D_PP_COMPOSE_REPEAT(depth, seq1, seq2, _0, count1, count2)  A3D_PP_INT_COMMA_IF(depth) A3D_PP_AT(seq1, A3D_PP_SUB1(count1)) A3D_PP_AT(seq2, count2)


#define A3D_PP_COMPOSE3_CONDI(depth, seq1, seq2, seq3, size2, size3, count1, count2, count3) A3D_PP_INT_BOOL(count1)

#define A3D_PP_COMPOSE3_UPDATE(depth, seq1, seq2, seq3, size2, size3,count1, count2, count3)                                \
			seq1, seq2, seq3, size2, size3,                                                                                 \
			A3D_PP_INT_IF(A3D_PP_ADD( count3, count2), count1, A3D_PP_SUB1(count1)),                                        \
			A3D_PP_INT_IF(count2, A3D_PP_INT_IF(count3, count2, A3D_PP_SUB1(count2)), A3D_PP_INT_IF(count3, count2, size2)),\
			A3D_PP_INT_IF(count3, A3D_PP_SUB1(count3), size3)
#define A3D_PP_COMPOSE3_REPEAT(depth, seq1, seq2, seq3, size2, size3, count1, count2, count3) A3D_PP_INT_COMMA_IF(depth) A3D_PP_AT(seq1, A3D_PP_SUB1(count1)) A3D_PP_AT(seq2, count2) A3D_PP_AT(seq3, count3)
			