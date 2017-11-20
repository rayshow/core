#include<cstdio>
#include<utility>
#include<memory>
#include<string>
#include<typeinfo>
#include<iostream>
#include<type_traits>
#include<assert.h>
#include<ccdk/mpl/type_traits.h>
#include<ccdk/mpl/base/literals.h>

using namespace ccdk::mpl;
template<typename T, typename=void> struct test_when :test_when<T, when > {};
template<typename T, bool condi>    struct test_when<T, match_default<condi> >{ static constexpr int value = 1; };
template<typename T> struct test_when<T, match< is_void<T>> > { static constexpr int value = 2; };
template<typename T> struct test_when<T, match_one< is_float<T>, is_integer<T> >> { static constexpr int value = 3; };
template<typename T> struct test_when<T, match_both< is_top_const<T>, is_pointer<T> >> { static constexpr int value = 4;};
template<typename T> static constexpr int test_when_v = test_when<T>::value;

template<typename T, T t>
struct test_float {};

struct test_tag { typedef int tag; };

using namespace literals;



int main()
{
	AssertTrue(has_inner_tag<test_tag>::value);
	DebugNewTitle("test when dispatch");
	AssertTrue(test_when_v<int> == 3);   //analyizer's error, not compiler
	AssertTrue(test_when_v<float> == 3);
	AssertTrue(test_when_v<void> == 2);
	AssertTrue(test_when_v<const int*> == 4);
	AssertTrue(test_when_v<nullptr_t> == 1);

	//test parse literal integer
	DebugNewTitle("test literals");
	AssertTrue(11_ci == 11);
	AssertTrue(-11_ci == -11);
	AssertTrue(0xff_ci == 255);
	AssertTrue(-0xff_ci == -255);
	AssertTrue(010_ci == 8);
	AssertTrue(-010_ci == -8);
	AssertTrue(-0b00000111_ci == -7);
	
	//test tuple 
	DebugNewTitle("test tuple operation")
	
	constexpr tuple<int, char, double, float> tuple1(1, 'a', 2.0, 1.0f);
	const char* aa = "hello";
	const tuple<int, char, std::string, float> tuple2( 2 ,'a', "hello", 1.0f);
	tuple<int, char, std::string, float> tuple3(3, 'a', aa, 1.0f);
	auto tuple4{ tuple3 };
	tuple<float, int, std::string, double> tuple5{ tuple3 };
	DebugSubTitle("test get elements");
	DebugValue(tuple3[0_th]);
	DebugValue(tuple3[1_th]);
	DebugValue(tuple3[2_th]);
	DebugValue(tuple3[3_th]);
	RuntimeAssertTrue(tuple2[int_<0>{}] == 2);
	RuntimeAssertTrue(tuple3[int_<0>{}] == 3);
	tuple3[int_<0>{}] = -1;
	RuntimeAssertTrue(tuple3[int_<0>{}] == -1);
	DebugValue(tuple3[int_<0>{}]);
	DebugValue(tuple3[int_<1>{}]);
	DebugValue(tuple3[int_<2>{}]);
	DebugValue(tuple3[int_<3>{}]);

	DebugSubTitle("debug type");
	DebugValueTypeName(tuple1[1_th]);
	DebugValueTypeName(tuple2[1_th]);
	DebugValueTypeName(tuple3[1_th]);
	DebugValueTypeName(util::move(tuple3)[1_th]);

	DebugSubTitle("debug assign element");
	DebugValue(tuple3[2_th]);
	tuple3[2_th] = "hello, world";
	DebugValue(tuple3[2_th]);
	std::string move_from = util::move(tuple3)[2_th];
	DebugValue(tuple3[2_th]);
	DebugValue(move_from);

	DebugSubTitle("debug tuple concat");

	char a = 'a';
	tuple<char, char, std::string, float> tuple6{ a, 'b', "hello", 1.0f};
	tuple<char, int, float, std::string >  tuple7{ 'c', 2, 2.5f, "world" };
	tuple<char> tuple8{ a}; 
	tuple<float> tuple9{ 2.0f };

	auto tuple10 = tuple8 + tuple9;
	char acc = 'c';
	const char dcc = 'b';
	auto tuple11 = tuple10 + acc;
	auto tuple12 = dcc + tuple11;
	DebugValueTypeName(tuple10);
	DebugValueTypeName(tuple11);
	DebugValueTypeName(tuple12);
	DebugValueTypeAndValue(tuple12[0_th]);
	DebugValueTypeAndValue(tuple12[1_th]);
	DebugValueTypeAndValue(tuple12[2_th]);
	DebugValueTypeAndValue(tuple12[3_th]);
	tuple12[3_th] = 'd';
	DebugValue(acc);


	getchar();
	return 0;
}