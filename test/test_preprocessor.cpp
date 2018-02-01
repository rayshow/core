#include<stdio.h>
#include<iostream>
#include<cassert>
#include<string>
#include<typeinfo>

#include<ccdk/preprocessor/stringize.h>
#include<ccdk/preprocessor/uint8_div.h>
#include<ccdk/preprocessor/uint8_mul.h>
#include<ccdk/preprocessor/uint8_mod.h>
#include<ccdk/preprocessor/uint8_sub_n.h>
#include<ccdk/preprocessor/uint8_sub_one.h>
#include<ccdk/preprocessor/uint8_equal.h>
#include<ccdk/preprocessor/uint8_not_equal.h>
#include<ccdk/preprocessor/uint8_greater_equal.h>
#include<ccdk/preprocessor/uint8_less_equal.h>
#include<ccdk/preprocessor/uint8_greater.h>
#include<ccdk/preprocessor/uint8_if.h>
#include<ccdk/preprocessor/while.h>
#include<ccdk/preprocessor/seq_compose.h>
#include<ccdk/preprocessor/seq_foreach_item.h>
#include<ccdk/preprocessor/seq_foreach_tuple.h>
#include<ccdk/preprocessor/range_prefix.h>
#include<ccdk/preprocessor/range_wrap.h>
#include<ccdk/preprocessor/range_call.h>
#include<ccdk/preprocessor/range_alias.h>
#include<ccdk/preprocessor/seq_unpack.h>
using namespace std;

#define  OUTPUT(a, b) cout << "  "<< a << CCDK_PP_STRINGIZE(b) <<endl << endl; 

int main()
{	
	//test macro + - * / %
	static_assert(CCDK_PP_ADD1(2) == 3, "");
	static_assert(CCDK_PP_ADD1(255) == 255, "");
	static_assert(CCDK_PP_ADD(34, 100) == 134, "");
	static_assert(CCDK_PP_ADD(255, 4) == 255, "");
	static_assert(CCDK_PP_SUB1(0) == 0, "");
	static_assert(CCDK_PP_SUB1(1) == 0, "");
	static_assert(CCDK_PP_SUB(255, 18) == 237, "");
	static_assert(CCDK_PP_SUB(17, 18) == 0, "");
	static_assert(CCDK_PP_MUL(1, 5) ==  5, "");
	static_assert(CCDK_PP_MUL(13, 4) == 52, "");
	static_assert(CCDK_PP_MUL(15, 5) == 75, "");
	static_assert(CCDK_PP_MUL(15, 17) == 255, ""); //mul   out of range
	static_assert(CCDK_PP_MUL(16, 17) == 255, ""); //shift out of range
	static_assert(CCDK_PP_DIV(15, 5) == 3,"");
	static_assert(CCDK_PP_DIV(23, 4) == 5, "");
	static_assert(CCDK_PP_DIV(23, 24) == 0, "");
	static_assert(CCDK_PP_MOD(15, 5) == 0, "");
	static_assert(CCDK_PP_MOD(23, 4) == 3, "");
	static_assert(CCDK_PP_MOD(23, 24) == 23, "");

	//test macro ==,!=,>,>=,<,<=
	static_assert(!CCDK_PP_EQUAL(255, 16), "");
	static_assert( CCDK_PP_EQUAL(16, 16), "");
	static_assert( CCDK_PP_NEQUAL(17, 16), "");
	static_assert(!CCDK_PP_NEQUAL(16, 16), "");
	static_assert( CCDK_PP_GREATER(17, 16), "");
	static_assert(!CCDK_PP_GREATER(16, 16), "");
	static_assert(!CCDK_PP_GREATER(15, 16), "");
	static_assert( CCDK_PP_GEQUAL(17, 16), "");
	static_assert( CCDK_PP_GEQUAL(16, 16), "");
	static_assert(!CCDK_PP_GEQUAL(15, 16), "");
	static_assert(!CCDK_PP_LESS(17, 16), "");
	static_assert(!CCDK_PP_LESS(16, 16), "");
	static_assert( CCDK_PP_LESS(15, 16), "");
	static_assert(!CCDK_PP_LEQUAL(17, 16), "");
	static_assert( CCDK_PP_LEQUAL(16, 16), "");
	static_assert( CCDK_PP_LEQUAL(15, 16), "");

	//test macro sequence operation
#define FOREACH_CALL(it,...) |it|
#define TEST_TUPLE(First, Second, ...)   |First,Second,__VA_ARGS__|
#define CCDK_PP_TEST_FORE_INDEX(Index, ForeIndex, Split) Index ForeIndex Split

	static_assert(3 == CCDK_PP_SIZE((1, 2, 3)), "seq size failed");
	static_assert(5 == CCDK_PP_AT((4, 5, 6), 1), "seq number failed");
	OUTPUT("0. seq expand: \n", CCDK_PP_RANGE_CALL(1,4,1, FOREACH_CALL, CCDK_PP_EMPTY));
	OUTPUT("1. seq expand: \n", CCDK_PP_EXPAND(CCDK_PP_EXPAND((1, 2, 3), 4),5));
	OUTPUT("2. seq unpack: \n", CCDK_PP_UNPACK((1, 2, 3)));
	OUTPUT("3. seq compose: \n", CCDK_PP_COMPOSE((1, 2), (3, 4)));
	OUTPUT("4. seq compose size: \n", CCDK_PP_SIZE(CCDK_PP_COMPOSE((1, 2), (3, 4))));
	OUTPUT("5. seq compose at: \n", CCDK_PP_AT(CCDK_PP_COMPOSE((1, 2), (3, 4)), 1));

	OUTPUT("6. seq compose 3: \n", CCDK_PP_COMPOSE3((1, 2), (3, 4), (5,6) ));
	OUTPUT("7. seq compose with ex-item: \n", CCDK_PP_COMPOSE_EX((unsigned, signed), (char, int), void ) );
	OUTPUT("8. seq compose3 with ex-item: \n", CCDK_PP_COMPOSE3_EX((unsigned, signed), (char, int),(&,&&), void));
	OUTPUT("9. seq foreach call: \n", CCDK_PP_FOREACH_ITEM(FOREACH_CALL, (int, char, short, float)));
	OUTPUT("10. seq foreach with compose-ex: \n", CCDK_PP_FOREACH_ITEM(FOREACH_CALL, CCDK_PP_COMPOSE_EX((const, volatile), (&&, &), &)));
	OUTPUT("11. seq foreach tuple: \n ", CCDK_PP_FOREACH_TUPLE(TEST_TUPLE, ((1, 2), (3, 4), (CCDK_PP_NULL, p, 3), (0, CCDK_PP_NULL, 4), (1, 1, 4), (2, 3, 4)), -));
	
	//test macro range
	OUTPUT("12. for range prefix: \n", CCDK_PP_RANGE_PREFIX(typename T, 2, 0, (,)));
	OUTPUT("13. for range prefix with step: \n", CCDK_PP_RANGE_PREFIX_STEP(typename T, 0, 4, 2, (,)));
	OUTPUT("14. for range wrap: \n", CCDK_PP_RANGE_WRAP(typename T, 2, 0, = ingore_t, (,)));
	OUTPUT("15. for range alias: \n", CCDK_PP_RANGE_ALIAS(typedef A, ::type, B, 0, 2, (;)));
	OUTPUT("16. for range chain alias: \n", CCDK_PP_RANGE_CHAIN_ALIAS(typedef A, ::type, B, 1, 3, (;)));
	OUTPUT("17. for range chain call: \n", CCDK_PP_RANGE_CHAIN_CALL(1, 3, CCDK_PP_TEST_FORE_INDEX, (;)));

#define ADD_SIGNED(a,...) cout << typeid(a).name() <<endl;
	CCDK_PP_FOREACH_ITEM(ADD_SIGNED, CCDK_PP_COMPOSE((char, short, int, long, long long), (, const, volatile, const volatile)));


	cout << "  18. test  while loop:" << endl;
#define WHILE_CONDI2(depth, down, up) CCDK_PP_INT_IF(down, 1, 0)           // down !=0
#define WHILE_LOOP(depth, down, up)  CCDK_PP_SUB1(down), CCDK_PP_ADD1(up)   //++down, --up
#define WHILE_REPEAT(depth, down, up)    cout <<"       "<<" depth:"<<depth<< " down: " << down << " up: " << up << endl;
	CCDK_PP_WHILE(WHILE_CONDI2, WHILE_LOOP, WHILE_REPEAT, 20, 0);
	getchar();
	return 0;
}
