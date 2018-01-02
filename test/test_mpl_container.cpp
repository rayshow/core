#include<cstdio>
#include<cstring>
#include<utility>
#include<memory>
#include<string>
#include<typeinfo>
#include<iostream>
#include<type_traits>
#include<assert.h>
#include<ccdk/mpl/type_traits.h>
#include<ccdk/mpl/fusion/string_literial.h>
#include<ccdk/mpl/fusion/any.h>
#include<ccdk/mpl/fusion/varient.h>
#include<ccdk/mpl/base/arg_pack_find_index.h>
#include<ccdk/mpl/base/val_pack_max.h>
#include<ccdk/mpl/type_traits/max_align_t.h>
#include<ccdk/type.h>
#include<ccdk/mpl/fusion/imap.h>

using namespace ccdk;
using namespace ccdk::mpl;
using namespace ccdk::mpl::literals;
using namespace ccdk::mpl::fs;

template<typename... Args>
void test_ref(Args&&... args)
{
	reference_args<Args...> a{util::forward<Args>(args)...};
	DebugValue(a.template at<1>());


	DebugValue("\n");
	const reference_args<Args...> ca{ util::forward<Args>(args)... };
	DebugValue(ca.template at<1>());



	DebugValue("\n");
	closure_args<sizeof...(Args),Args...> b{ util::forward<Args>(args)... };
	DebugValue(b.template at<1>());


	DebugValue("\n");
	const closure_args<sizeof...(Args),Args...> cb{ util::forward<Args>(args)... };
	DebugValue(cb.template at<1>());

}


int main()
{
	DebugNewTitle("any");
	any any1{ 3 };
	RuntimeAssertTrue(any1.to<int>() == 3);
	any1 = "hello,world";
	RuntimeAssertTrue(any1.to<const char*>() == "hello,world");
	any1 = test_copy_t{3};
	RuntimeAssertTrue(any1.to<test_copy_t>().v == 3);
	any1 = 1.3f;
	RuntimeAssertTrue(any1.to<float>() == 1.3f);
	//try { any1.to<int>(); }
	//catch (bad_any_cast& ex) { DebugValue("base any cast"); }


	DebugNewTitle("varient")
	test_destruct tda("hello,world");
	test_destruct tdb("c++");
	varient<double, int,float, test_destruct > la{ 1.4 };

	int size = 0;
	DebugValue(sizeof(la));
	DebugValue((size = la.memory_size));
	DebugValue(sizeof(test_destruct));
	RuntimeAssertTrue(la.to<double>() == 1.4);
	la = tda;
	RuntimeAssertTrue(!strcmp( la.to<test_destruct>().val,"hello,world"));
	la = tdb;
	RuntimeAssertTrue(!strcmp(la.to<test_destruct>().val,"c++"));
	la = 1.1f;
	RuntimeAssertTrue(la.to<float>() == 1.1f);

	DebugNewTitle("ref args");
	const char* str = "abc";
	const char* const instr = "fdsa";
	auto args = create_reference_args(1, "fdas", str, instr, test_copy_t{2});
	RuntimeAssertTrue(args.at<0>()==1);
	RuntimeAssertTrue(args.at<1>()=="fdas");
	RuntimeAssertTrue(args.at<2>()==str);
	RuntimeAssertTrue(args.at<3>() == instr);
	RuntimeAssertTrue(args.at<4>().v==2);

	/*constexpr string */
	DebugNewTitle("string literial");
	DebugValueTypeName(_literal("hello,world"));
	constexpr auto sl = _literal("hello,world");
	DebugValue(sl);
	AssertTrue(sl[3_th]=='l');
	AssertTrue(sl.find_first('l')==2);
	AssertTrue(sl.find_last('l')==9);
	AssertTrue((sl.substr<2, 5>()) == _literal("llo"));
	AssertTrue( sl.replace<sl.find_first('w')>('F') == _literal("hello,Forld") );

	//test tuple 
	DebugNewTitle("test tuple operation")	
	DebugSubTitle("test get elements");
	tuple<int, float, const char*, std::string> tuple1{ 2, 1.2f, "hello,world", "c++" };
	RuntimeAssertTrue(tuple1[0_th] == 2);
	RuntimeAssertTrue(tuple1[1_th] == 1.2f);
	RuntimeAssertTrue(tuple1[2_th] == "hello,world");
	RuntimeAssertTrue(tuple1[3_th] == "c++");
	tuple1[0_th] = -1;
	tuple1[2_th] = "my tuple";
	RuntimeAssertTrue(tuple1[2_th] == "my tuple");


	DebugSubTitle("debug tuple concat");
	tuple<char,int> tuple2{ 'a',1}; 
	tuple<float> tuple3{ 2.23f };
	auto tuple4 = tuple2 + tuple3;
	RuntimeAssertTrue(tuple4[0_th]=='a');
	RuntimeAssertTrue(tuple4[1_th]==1);
	RuntimeAssertTrue(tuple4[2_th]==2.23f);

	DebugSubTitle("debug tuple push back push front");
	auto tuple5 = tuple1.push_back('B', "cd").push_front('F', 1.222f);
	RuntimeAssertTrue(tuple5[0_th] == 'F');
	RuntimeAssertTrue(tuple5[1_th] == 1.222f);
	RuntimeAssertTrue(tuple5[6_th] == 'B');
	RuntimeAssertTrue(tuple5[7_th] == "cd");

	DebugSubTitle("debug tuple pop back pop front");
	auto tuple6 = tuple1.pop_back().pop_front();
	RuntimeAssertTrue(tuple6[0_th] == 1.2f);
	RuntimeAssertTrue(tuple6[1_th] == "my tuple");

	DebugSubTitle("test insert tuple");
	auto tuple7 = tuple1.insert<0>(1.555).insert<3>(true);
	RuntimeAssertTrue(tuple7[0_th] == 1.555);
	RuntimeAssertTrue(tuple7[3_th] == true);

	DebugSubTitle("test replace tuple");
	auto tuple8 = tuple1.replace<2, 3>('a', "bc");
	RuntimeAssertTrue(tuple8[2_th] == 'a');
	RuntimeAssertTrue(tuple8[3_th] == "bc");

	DebugSubTitle("test erase tuple");
	
	tuple<int, test_copy_t, float, std::string> tuple9{1,1,1.2f,"big world"};
	auto tuple10 = tuple9.erase<1, 3>();
	RuntimeAssertTrue(tuple10[0_th] == 1);
	RuntimeAssertTrue(tuple10[1_th] == std::string("big world"));

	getchar();
	return 0;
}