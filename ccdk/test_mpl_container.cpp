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
#include<ccdk/mpl/container/constexpr_string.h>
#include<ccdk/mpl/container/string_view.h>
#include<ccdk/type.h>

using namespace ccdk::mpl;
using namespace ccdk;
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

template<typename T, int... args>
struct test_char_arr {};

template<uint32 N, typename T, int... args>
struct test_char_arr< T[N],args... > 
{
	int a[N];
	constexpr test_char_arr(const int* b) :a{ b[args]... } {}

	template<typename T,T index>
	constexpr auto operator[](integer_<T,index>) const noexcept
	{
		return a[index];
	}
};

constexpr int const_int(int size)
{
	return size;
}

//constexpr auto test(const char* str,int size)
//{
//	constexpr int a4[const_int(size)] = { 3,2,3,4 };
//	constexpr test_char_arr<int[4], 0, 1, 2, 3> arr{ a4 };
//}



int main()
{
	constexpr char ad = "abcd"_cs;
	uint_<"abc"_cs>{};
	printf("%c", ad);
	"fdasf"_cc;

	//arr.print();
	typedef int int2[2];
	constexpr int2 a = { 1,2 };
	constexpr int2 b = { a[0],a[1] };
	//constexpr int2 c = { a };

	DebugValueTypeName("fdsa");

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
	//auto tuple10 = tuple8 | tuple9;
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

	constexpr constexpr_string<char,'a', 'b', 'c', 'd'> str{};
	str[1_th];
	str.length();
	//"fdsa"_cs;
	
	

	constexpr char *aptr = 0;
	constexpr string_view cstr{ "abcd" };
	constexpr int find = cstr.find('b');
	uint_<find>{};
	
	constexpr int aaaa[2] = { 1,2 };
	

	//AssertTrue(str == "abcd");
	constexpr auto c = str.find<'e'>();
	constexpr auto d = str.replace<'f'>(str.find<'c'>());

	constexpr char*p = 0;
	DebugValueTypeName(d);
	DebugValue(d.c_str());
	DebugValue((int)c);


	getchar();
	return 0;
}