#include<stdio.h>
#include<iostream>
#include<cassert>
#include<string>
#include<typeinfo>

#include<core/preprocessor/stringize.h>
#include<core/preprocessor/uint8_div.h>
#include<core/preprocessor/uint8_mul.h>
#include<core/preprocessor/uint8_mod.h>
#include<core/preprocessor/uint8_sub_n.h>
#include<core/preprocessor/uint8_sub_one.h>
#include<core/preprocessor/uint8_equal.h>
#include<core/preprocessor/uint8_not_equal.h>
#include<core/preprocessor/uint8_greater_equal.h>
#include<core/preprocessor/uint8_less_equal.h>
#include<core/preprocessor/uint8_greater.h>
#include<core/preprocessor/uint8_if.h>
#include<core/preprocessor/while.h>
#include<core/preprocessor/seq_compose.h>
#include<core/preprocessor/seq_foreach_item.h>
#include<core/preprocessor/seq_foreach_tuple.h>
#include<core/preprocessor/range_prefix.h>
#include<core/preprocessor/range_wrap.h>
#include<core/preprocessor/range_alias.h>
#include<core/preprocessor/seq_unpack.h>
using namespace std;

#define  OUTPUT(a, b) cout << "  "<< a << A3D_PP_STRINGIZE(b) <<endl << endl; 

int main()
{	
	//test macro + - * / %
	static_assert(A3D_PP_ADD1(2) == 3, "");
	static_assert(A3D_PP_ADD1(255) == 255, "");
	static_assert(A3D_PP_ADD(34, 100) == 134, "");
	static_assert(A3D_PP_ADD(255, 4) == 255, "");
	static_assert(A3D_PP_SUB1(0) == 0, "");
	static_assert(A3D_PP_SUB1(1) == 0, "");
	static_assert(A3D_PP_SUB(255, 18) == 237, "");
	static_assert(A3D_PP_SUB(17, 18) == 0, "");
	static_assert(A3D_PP_MUL(1, 5) ==  5, "");
	static_assert(A3D_PP_MUL(13, 4) == 52, "");
	static_assert(A3D_PP_MUL(15, 5) == 75, "");
	static_assert(A3D_PP_MUL(15, 17) == 255, ""); //mul   out of range
	static_assert(A3D_PP_MUL(16, 17) == 255, ""); //shift out of range
	static_assert(A3D_PP_DIV(15, 5) == 3,"");
	static_assert(A3D_PP_DIV(23, 4) == 5, "");
	static_assert(A3D_PP_DIV(23, 24) == 0, "");
	static_assert(A3D_PP_MOD(15, 5) == 0, "");
	static_assert(A3D_PP_MOD(23, 4) == 3, "");
	static_assert(A3D_PP_MOD(23, 24) == 23, "");

	//test macro ==,!=,>,>=,<,<=
	static_assert(!A3D_PP_EQUAL(255, 16), "");
	static_assert( A3D_PP_EQUAL(16, 16), "");
	static_assert( A3D_PP_NEQUAL(17, 16), "");
	static_assert(!A3D_PP_NEQUAL(16, 16), "");
	static_assert( A3D_PP_GREATER(17, 16), "");
	static_assert(!A3D_PP_GREATER(16, 16), "");
	static_assert(!A3D_PP_GREATER(15, 16), "");
	static_assert( A3D_PP_GEQUAL(17, 16), "");
	static_assert( A3D_PP_GEQUAL(16, 16), "");
	static_assert(!A3D_PP_GEQUAL(15, 16), "");
	static_assert(!A3D_PP_LESS(17, 16), "");
	static_assert(!A3D_PP_LESS(16, 16), "");
	static_assert( A3D_PP_LESS(15, 16), "");
	static_assert(!A3D_PP_LEQUAL(17, 16), "");
	static_assert( A3D_PP_LEQUAL(16, 16), "");
	static_assert( A3D_PP_LEQUAL(15, 16), "");

	//test macro sequence operation
#define FOREACH_CALL(it,...) |it|
#define TEST_TUPLE(First, Second, ...)   |First,Second,__VA_ARGS__|
#define A3D_PP_TEST_FORE_INDEX(Index, ForeIndex, Split) Index ForeIndex Split

	static_assert(3 == A3D_PP_SIZE((1, 2, 3)), "seq size failed");
	static_assert(5 == A3D_PP_AT((4, 5, 6), 1), "seq number failed");
	OUTPUT("1. seq expand: \n", A3D_PP_EXPAND(A3D_PP_EXPAND((1, 2, 3), 4),5));
	OUTPUT("2. seq unpack: \n", A3D_PP_UNPACK((1, 2, 3)));
	OUTPUT("3. seq compose: \n", A3D_PP_COMPOSE((1, 2), (3, 4)));
	OUTPUT("4. seq compose size: \n", A3D_PP_SIZE(A3D_PP_COMPOSE((1, 2), (3, 4))));
	OUTPUT("5. seq compose at: \n", A3D_PP_AT(A3D_PP_COMPOSE((1, 2), (3, 4)), 1));

	OUTPUT("6. seq compose 3: \n", A3D_PP_COMPOSE3((1, 2), (3, 4), (5,6) ));
	OUTPUT("7. seq compose with ex-item: \n", A3D_PP_COMPOSE_EX((unsigned, signed), (char, int), void ) );
	OUTPUT("8. seq compose3 with ex-item: \n", A3D_PP_COMPOSE3_EX((unsigned, signed), (char, int),(&,&&), void));
	OUTPUT("9. seq foreach call: \n", A3D_PP_FOREACH_ITEM(FOREACH_CALL, (int, char, short, float)));
	OUTPUT("10. seq foreach with compose-ex: \n", A3D_PP_FOREACH_ITEM(FOREACH_CALL, A3D_PP_COMPOSE_EX((const, volatile), (&&, &), &)));
	OUTPUT("11. seq foreach tuple: \n ", A3D_PP_FOREACH_TUPLE(TEST_TUPLE, ((1, 2), (3, 4), (A3D_PP_NULL, p, 3), (0, A3D_PP_NULL, 4), (1, 1, 4), (2, 3, 4)), -));
	
	//test macro range
	OUTPUT("12. for range prefix: \n", A3D_PP_RANGE_PREFIX(typename T, 2, 0, (,)));
	OUTPUT("13. for range prefix with step: \n", A3D_PP_RANGE_PREFIX_STEP(typename T, 0, 4, 2, (,)));
	OUTPUT("14. for range wrap: \n", A3D_PP_RANGE_WRAP(typename T, 2, 0, = ingore_t, (,)));
	OUTPUT("15. for range alias: \n", A3D_PP_RANGE_ALIAS(typedef A, ::type, B, 0, 2, (;)));
	OUTPUT("16. for range chain alias: \n", A3D_PP_RANGE_CHAIN_ALIAS(typedef A, ::type, B, 1, 3, (;)));
	OUTPUT("17. for range chain call: \n", A3D_PP_RANGE_CHAIN_CALL(1, 3, A3D_PP_TEST_FORE_INDEX, (;)));

#define ADD_SIGNED(a,...) cout << typeid(a).name() <<endl;
	A3D_PP_FOREACH_ITEM(ADD_SIGNED, A3D_PP_COMPOSE((char, short, int, long, long long), (, const, volatile, const volatile)));


	cout << "  18. test  while loop:" << endl;
#define WHILE_CONDI2(depth, down, up) A3D_PP_INT_IF(down, 1, 0)           // down !=0
#define WHILE_LOOP(depth, down, up)  A3D_PP_SUB1(down), A3D_PP_ADD1(up)   //++down, --up
#define WHILE_REPEAT(depth, down, up)    cout <<"       "<<" depth:"<<depth<< " down: " << down << " up: " << up << endl;
	A3D_PP_WHILE(WHILE_CONDI2, WHILE_LOOP, WHILE_REPEAT, 20, 0);
	getchar();
	return 0;
}
