 #include<cstdio>
#include<utility>
#include<memory>
#include<string>
#include<typeinfo>
#include<iostream>
#include<type_traits>
#include<assert.h>
#include<ccdk/mpl/type_traits.h>
#include<ccdk/mpl/fusion/string_literial.h>
#include<ccdk/mpl/fusion/ref_tuple.h>
#include<ccdk/mpl/fusion/any.h>
#include<ccdk/mpl/fusion/limit_any.h>
#include<ccdk/mpl/fusion/auto_any.h>
#include<ccdk/mpl/base/arg_pack_find_index.h>
#include<ccdk/type.h>

using namespace ccdk;
using namespace ccdk::mpl;
using namespace ccdk::mpl::literals;
using namespace ccdk::mpl::fs;


int main()
{
	DebugNewTitle("any");

	any ai1 = 3;
	int ri = 4;
	const int cri = 5;
	any ai2 = ri;
	any ai3 = cri;
	const any ai4 = 1;

	DebugValueTypeName(ai1.to<int>());
	DebugValueTypeName(ai4.to<int>());
	DebugValueTypeName(util::move(ai1).to<int>());

	DebugValueTypeName(ai4.to_pointer<int>());
	DebugValueTypeName(ai1.to_pointer<int>());

	try
	{
		DebugValueTypeName(ai1.to<char>());
	}
	catch (const std::exception& ex)
	{
		DebugValue(ex.what());
	}

	DebugNewTitle("limit any")
	test_destruct tda("hello,world");
	test_destruct tdb("c++");
	DebugValue("limit_any");
	limit_any_impl<make_indice<4>, double, int, char, test_destruct > la{ 1.0 };
	DebugValue(sizeof(la));
	DebugValue(la.to<double>());
	la = tda;
	DebugValue(la.to<test_destruct>().val);
	la = tdb;
	DebugValue(la.to<test_destruct>().val);
	la = 1.0;
	DebugValue(la.to<double>());

	getchar();
	return 0;

	DebugNewTitle("ref tuple");
	const char* str = "abc";
	const char* const instr = "fdsa";
	auto ref_tuple = create_ref_tuple(1, "fdas", str, instr);
	DebugTypeName<decltype(ref_tuple.at<1>())>();
	DebugValue(ref_tuple.at<0>());
	DebugValue(ref_tuple.at<1>());
	DebugValue(ref_tuple.at<2>());
	DebugValue(ref_tuple.at<3>());

	

 	//DebugValue(ai.to<int>());


	

	DebugNewTitle("string literial");
	DebugValueTypeName(_literal("hello,world"));
	constexpr auto sl = _literal("hello,world");
	DebugValue(sl);
	DebugValue(sl[3_th]);
	
	DebugValue(sl.find_first('l'));
	DebugValue(sl.find_last('l'));
	DebugValue(sl.substr<0, 3>());
	AssertTrue((sl.substr<2, 5>()) == _literal("llo"));
	DebugValue(sl.replace<sl.find_first('w')>('F'));

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
	//constexpr tuple<int, char, double, float> tuple1(1, 'a', 2.0, 1.0f);
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
	//DebugValueTypeName(tuple1[1_th]);
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
	char acc = 'a';
	tuple<char, char, std::string, float> tuple6{ acc, 'b', "hello", 1.0f};
	tuple<char, int, float, std::string >  tuple7{ 'c', 2, 2.5f, "world" };
	tuple<char,int> tuple8{ acc,1}; 
	tuple<float> tuple9{ 2.23f };
	auto tuple10 = tuple8 | tuple9;
	//DebugValueTypeAndValue(tuple10[0_th]);
	//DebugValueTypeAndValue(tuple10[1_th]);
	//DebugValueTypeAndValue(tuple10[2_th]);

	//DebugSubTitle("debug tuple push back push front");
	//char acc = 'c'; const char dcc = 'b';
	//auto tuple11 = tuple10.push_back('B', acc );
	//auto tuple12 = tuple11.push_front('F', 1.222f);
	//DebugValueTypeAndValue(tuple12[0_th]);
	//DebugValueTypeAndValue(tuple12[1_th]);
	//DebugValueTypeAndValue(tuple12[2_th]);
	//DebugValueTypeAndValue(tuple12[3_th]);
	//DebugValueTypeAndValue(tuple12[4_th]);
	//DebugValueTypeAndValue(tuple12[5_th]);
	//DebugValueTypeAndValue(tuple12[6_th]);
	//DebugSubTitle("debug tuple pop back pop front");
	//auto tuple13 = tuple12.pop_back();
	//auto tuple14 = tuple13.pop_front();
	//DebugValueTypeAndValue(tuple14[0_th]);
	//DebugValueTypeAndValue(tuple14[1_th]);
	//DebugValueTypeAndValue(tuple14[2_th]);

	//DebugValueTypeName(tuple10);
	//DebugValueTypeName(tuple11);
	//DebugValueTypeName(tuple12);
	//DebugValueTypeAndValue(tuple12[0_th]);
	//DebugValueTypeAndValue(tuple12[1_th]);
	//DebugValueTypeAndValue(tuple12[2_th]);
	//DebugValueTypeAndValue(tuple12[3_th]);
	//tuple12[3_th] = 'd';
	//DebugValue(acc);

	//DebugSubTitle("test tuple pop ");
	//auto tuple13 = tuple12.pop_back();
	//auto tuple14 = tuple13.pop_front();
	//DebugValueTypeAndValue(tuple14[0_th]);
	//DebugValueTypeAndValue(tuple14[1_th]);

	//DebugSubTitle("test insert tuple");
	//auto tuple15 = tuple14.insert<1>(1.2f);



	getchar();
	return 0;
}