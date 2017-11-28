#include<cstdio>
#include<utility>
#include<memory>
#include<string>
#include<typeinfo>
#include<iostream>
#include<type_traits>
#include<cassert>
#include<functional>
#include<ccdk/mpl/type_traits.h>
#include<ccdk/mpl/container/arg_pack.h>
#include<ccdk/mpl/container/val_pack.h>
#include<ccdk/mpl/base/integer_.h>
#include<ccdk/mpl/base/int_.h>
#include<ccdk/mpl/base/dispatch_when.h>
#include<ccdk/mpl/function/arg.h>
#include<ccdk/mpl/function/function.h>

using namespace ccdk;
using namespace ccdk::mpl;
using namespace ccdk::mpl::literals;


// if T& version constructor not exists, will call const T& version
struct test_copy_constructor
{
	test_copy_constructor() = default;
    test_copy_constructor(test_copy_constructor&& t) { DebugValue("move ctor"); }
	test_copy_constructor(const test_copy_constructor& t) { DebugValue("const copy ctor"); }

	void operator=(test_copy_constructor& t) { DebugValue("copy assign"); }
	void operator=(test_copy_constructor&& t) { DebugValue("move assign"); }
	void operator=(const test_copy_constructor& t) { DebugValue("const copy assign"); }
};





template<typename T>
struct test_copy_constructor2
{
	test_copy_constructor2() = default;

	test_copy_constructor2(test_copy_constructor2& t) { DebugValue("2 copy ctor"); }

	test_copy_constructor2(test_copy_constructor2&& t) { DebugValue("2 move ctor"); }

	test_copy_constructor2(const test_copy_constructor2& t) { DebugValue("2 const copy ctor"); }

	template<typename U>
	test_copy_constructor2(test_copy_constructor2<U>& t) { DebugValue("2 tmpl copy ctor"); }

	template<typename U>
	test_copy_constructor2(test_copy_constructor2<U>&& t) { DebugValue("2 tmpl move ctor"); }

	template<typename U>
	test_copy_constructor2(const test_copy_constructor2<U>& t) { DebugValue("2 tmpl const copy ctor"); }

	void operator=(test_copy_constructor2& t) { DebugValue("2 copy assign"); }

	void operator=(test_copy_constructor2&& t) { DebugValue("2 move assign"); }

	void operator=(const test_copy_constructor2& t) { DebugValue("2 const copy assign"); }

	template<typename U>
	void operator=(test_copy_constructor2<U>& t) { DebugValue("2 tmpl copy assign"); }

	template<typename U>
	void operator=(test_copy_constructor2<U>&& t) { DebugValue("2 tmpl move assign"); }

	template<typename U>
	void operator=(const test_copy_constructor2<U>& t) { DebugValue("2 tmpl const copy assign"); }
};

template<typename T, typename U, bool = is_lref_v<T>, bool = is_rref_v<T>>
struct adjust_from_this
{
	typedef remove_ref_t< U>& type;
};

template<typename T, typename U>
struct adjust_from_this<T, U, false, true>
{
	typedef remove_ref_t<U>&& type;
};

#define AdjustFromThis(v) static_cast< typename adjust_from_this<decltype(*this), decltype(v)>::type >(v);

template<typename T>
constexpr decltype(auto) test_forward(T&& t)
{
	DebugTypeName<T>();
	DebugValueTypeName(util::forward<T>(t));
	DebugFunctionName();
	return util::forward<T>(t);
}

//empty class optimization if V is empty
// V and U can be different type but U can be convert to V
template<typename T >
struct test_ctor 
{
	T v;
	constexpr test_ctor() :v{} {}

	//prefect forward
	template<typename U>
	constexpr test_ctor(U&& inU) : v(util::forward<U>(inU)) 
	{
		DebugFunctionName();
		DebugValue("&&");
	}

	template<typename U>
	constexpr test_ctor(const U& inU) : v(inU) 
	{
		DebugFunctionName();
		DebugValue("const &");
	}

};

struct IsIntOrNullptr
{
	template<typename T>
	struct apply :or_< is_integer<T>, is_nullptr_t<T>> {};
};

void test_normal(int a)
{
	DebugValue(a);
}

struct test_member_obj
{
	void operator()(const char* string)
	{
		DebugValue(string);
	}
};

template<typename T>
void test_fn_type(T n)
{
	DebugFunctionName();
	DebugTypeName<typename is_function_ptr<T>::type>();
	DebugTypeName<typename is_function_obj<T>::type>();
	DebugTypeName<typename is_mfn_ptr<T>::type>();
}

struct test_member_type
{
	int print(const char* h)
	{
		DebugValue(h);
		return 0;
	}
};


template<typename T>
void test(T&&)
{
	DebugTypeName<T>();
}


int main()
{
	const int tia = 1;
	int tib = 2;
	test(1);
	test(util::move(tib));
	test(tia);
	test(tib);

	DebugNewTitle("test dispatcher when");
	//constexpr int a = constexpr []() {return 0; };

	test_fn_type(test_normal);
	function<void(int)> fn1{ test_normal };
	fn1(1);
	
	test_fn_type(test_member_obj{});
	function<void(const char*)> fn2{ test_member_obj{} };
	fn2("hello,world");

	test_fn_type(&test_member_type::print);
	DebugTypeName<mfn_class_t<decltype(&test_member_type::print)>>();
	function<void(test_member_type&, const char*)> fn3{ &test_member_type::print };
	test_member_type tt;
	fn3(tt, "hhhh");


	getchar();
	return 0;

	DebugNewTitle("test val_pack");
	DebugTypeName<make_indice<0>>();
	DebugTypeName<make_indice<10>>();
	DebugTypeName<make_indice_from<0, 1>>();
	DebugTypeName<make_indice_from<4, 9>>();
	DebugTypeName<make_indice_ingore< 1, 3, 4>>();
	DebugTypeName<val_pack_create<char, 'a', 'e'>>();
	DebugTypeName<typename val_pack_split<uint32, 3, 5, make_indice_from<0, 7>>::type >();
	DebugValue(val_pack_at<uint32, 4, make_indice_from<1, 10>>);
	DebugValue(val_pack_find< uint32, 3, make_indice_from<3, 6>>);
	
	constexpr val_pack<char> pack{};
	constexpr auto pack2 = pack.push_back<'b', 'c'>();
	constexpr auto pack3 = pack2.push_front<'a'>();
	DebugValue(pack3[0_th]);
	DebugValue(pack3[2_th]);
	constexpr auto pack4 = pack3 | val_pack<char, 'd', 'e'>{};
	static_assert(pack4 == val_pack<char, 'a', 'b', 'c', 'd', 'e'>{}, "");
	pack4.insert<1, 'B', 'C'>();
	constexpr auto pack5 = val_pack<int, 0, 1>{}.insert<2, 2, 3>();
	constexpr auto pack6 = pack5.erase<1, 3>();
	constexpr auto pack7 = pack6.pop_front();
	constexpr auto pack8 = pack7.pop_back();
	constexpr auto pack9 = pack8.insert(0_th, val_pack<int, 2, 3, 4>{});
	constexpr auto pack10 = pack9.replace<1,2,12,13>();
	static_assert(pack10.contain<12>(),"");
	static_assert(!pack10.contain<11>(), "");
	DebugValueTypeName(pack);
	DebugValueTypeName(pack2);
	DebugValueTypeName(pack3);
	DebugValueTypeName(pack4);
	DebugValueTypeName(pack5);
	DebugValueTypeName(pack6);
	DebugValueTypeName(pack7);
	DebugValue(pack7.length());
	DebugValueTypeName(pack8);
	DebugValueTypeName(pack9);
	DebugValueTypeName(pack10);
	static_assert(pack8.empty(), "");

	//test arg pack
	DebugNewTitle("test arg pack");
	typedef arg_pack<int, short, long, char, float> ap;
	typedef arg_pack_split< 4, 1, ap> ingore_index;
	typedef arg_pack_split< 4, 0, ap> ingore_index2;
	DebugTypeName<typename ingore_index::head>();
	DebugTypeName<typename ingore_index::tail>();
	DebugTypeName<typename ingore_index2::head>();
	DebugTypeName<typename ingore_index2::tail>();

	DebugNewTitle("test tuple operation")
	constexpr tuple<int, char, double, float> tp1(1, 'a', 2.0, 1.0f);
	const char* aa = "hello";
	const tuple<int, char, std::string, float> tp2(2, 'a', "hello", 1.0f);
	tuple<int, char, std::string, float> tp3(3, 'a', aa, 1.0f);
	DebugValueTypeName(tp1);
	DebugValueTypeName(tp2);
	DebugValueTypeName(tp3);
	auto tp4{ tp3 };
	tuple<float, int, std::string, double> tp5{ tp3 };
	DebugSubTitle("test get elements");
	DebugValue(tp3[0_th]);
	DebugValue(tp3[1_th]);
	DebugValue(tp3[2_th]);
	DebugValue(tp3[3_th]);
	tp3[int_<0>{}] = -1;
	tp3[2_th] = "hello,world";
	DebugValue(tp3[int_<0>{}]);
	DebugValue(tp3[int_<1>{}]);
	DebugValue(tp3[int_<2>{}]);
	DebugValue(tp3[int_<3>{}]);
	DebugSubTitle("debug type");
	DebugValueTypeName(tp1[1_th]);
	DebugValueTypeName(tp2[1_th]);
	DebugValueTypeName(tp3[1_th]);
	DebugValueTypeName(util::move(tp3)[1_th]);

	DebugSubTitle("debug assign element");
	DebugValue(tp3[2_th]);
	tp3[2_th] = "hello, world";
	DebugValue(tp3[2_th]);
	std::string move_from = util::move(tp3)[2_th];
	DebugValue(tp3[2_th]);
	DebugValue(move_from);

	DebugSubTitle("debug tuple concat");
	char acc = 'a';
	tuple<char, char, std::string, float> tp6{ acc, 'b', "hello", 1.0f };
	tuple<char, int, float, std::string >  tp7{ 'c', 2, 2.5f, "world" };
	tuple<char, int> tp8{ acc,1 };
	tuple<float> tp9{ 2.23f };
	auto tp10 = tp8 | tp9;
	DebugTypeAndValue(tp10[0_th]);
	DebugTypeAndValue(tp10[1_th]);
	DebugTypeAndValue(tp10[2_th]);

	DebugSubTitle("debug tuple push back push front");
	char bcc = 'c'; 
	const char ccc = 'b';
	auto tp11 = tp10.push_back('B', bcc );
	auto tp12 = tp11.push_front(ccc, 1.222f);
	DebugTypeAndValue(tp12[0_th]);
	DebugTypeAndValue(tp12[1_th]);
	DebugTypeAndValue(tp12[2_th]);
	DebugTypeAndValue(tp12[3_th]);
	DebugTypeAndValue(tp12[4_th]);
	DebugTypeAndValue(tp12[5_th]);
	DebugTypeAndValue(tp12[6_th]);
	DebugSubTitle("debug tuple pop back pop front");
	DebugValue(tp12.length());
	auto tp13 = tp12.pop_back<3>();
	DebugValue(tp13.length());
	auto tp14 = tp13.pop_front<2>();
	DebugValue(tp14.length());
	DebugTypeAndValue(tp14[0_th]);
	DebugTypeAndValue(tp14[1_th]);

	DebugSubTitle("test insert tuple");
	auto tp15 = tp14.insert<1>(1.2f,"ffdf");
	DebugTypeAndValue(tp15[0_th]);
	DebugTypeAndValue(tp15[1_th]);
	DebugTypeAndValue(tp15[2_th]);
	DebugTypeAndValue(tp15[3_th]);
	DebugSubTitle("test erase tuple");
	auto tp16 = tp15.erase<1, 3>();
	DebugValue(tp16.length());
	DebugTypeAndValue(tp16[0_th]);
	DebugTypeAndValue(tp16[1_th]);
	
	//test forward
	DebugNewTitle("test forward");
	test_forward(0);
	DebugValueTypeName(test_forward(0));
	int ia = 1;
	test_forward(ia);
	DebugValueTypeName(test_forward(ia));
	const int ib = 2;
	test_forward(ib);
	DebugValueTypeName(test_forward(ib));
	test_forward("abc");
	DebugValueTypeName(test_forward("abc"));

	//test ctor
	DebugNewTitle("test template cotr");
	test_ctor<int> a11{1};
	int a = 1;
	test_ctor<int&> a22{a};
	DebugValue(a);
	DebugValue(a11.v);
	DebugValue(a22.v);
	a = 2;
	DebugValue(a);
	DebugValue(a11.v);
	DebugValue(a22.v);


	//test auto
	DebugNewTitle("test auto and decltype");
	auto ta = 27;     //deduce int
	auto tb(27);      //deduce int
	auto td{ 27 };    //deduce int
	auto tc = { 27 }; //deduce initalize_list<int>
	DebugTypeAndValue((ta)); // decltype((var)) preduce T&
	DebugTypeAndValue(ta);
	DebugTypeAndValue(tb);
	DebugValueTypeName(tc);
	DebugTypeAndValue(td);

	auto& tf = tb;
	decltype(auto) te = tf;
	
	DebugValueTypeName(tf);
	DebugValueTypeName(te);


	//constructor and assign
	DebugNewTitle(" normal constructor and assign test");
	test_copy_constructor t1{};
	const test_copy_constructor t2{};
	test_copy_constructor t3{ t1 };
	test_copy_constructor t4{ t2 };
	test_copy_constructor t5{ util::move(t1) };

	test_copy_constructor t6(t1);
	test_copy_constructor t7(t2);
	test_copy_constructor t8(util::move(t1));

	test_copy_constructor t9 = t1;
	test_copy_constructor t10 = t2;
	test_copy_constructor t11 = util::move(t1);
	test_copy_constructor t12{};
	t12 = t1;
	t12 = t2;
	t12 = util::move(t1);


	//constructor and assign
	DebugNewTitle(" same template constructor and assign test");
	test_copy_constructor2<int> u1{};
	const test_copy_constructor2<int> u2{};
	test_copy_constructor2<int> u3{ u1 };
	test_copy_constructor2<int> u4{ u2 };
	test_copy_constructor2<int> u5{ util::move(u1) };
	test_copy_constructor2<int> u6(u1);
	test_copy_constructor2<int> u7(u2);
	test_copy_constructor2<int> u8(util::move(u1));
	test_copy_constructor2<int> u9 = u1;
	test_copy_constructor2<int> u10 = u2;
	test_copy_constructor2<int> u11 = util::move(u1);
	test_copy_constructor2<int> u12{};
	u12 = u1;
	u12 = u2;
	u12 = util::move(u1);

	DebugNewTitle(" different template constructor and assign test");
	test_copy_constructor2<char> p3{ u1 };
	test_copy_constructor2<char> p4{ u2 };
	test_copy_constructor2<char> p5{ util::move(u1) };
	test_copy_constructor2<char> p6(u1);
	test_copy_constructor2<char> p7(u2);
	test_copy_constructor2<char> p8(util::move(u1));
	test_copy_constructor2<char> p9 = u1;
	test_copy_constructor2<char> p10 = u2;
	test_copy_constructor2<char> p11 = util::move(u1);
	test_copy_constructor2<char> p12{};
	p12 = u1;
	p12 = u2;
	p12 = util::move(u1);

	DebugNewTitle(" test member override.");

	struct test_override
	{
		int a = 0;
		int&   operator[](int) & { DebugValue(" & []"); DebugValueTypeName(this);  return a; }
		int&& operator[](int) && { DebugValue(" && []"); DebugValueTypeName(this);  return util::move(a); }
		const int&  operator[](int) const & { DebugValue(" const & []"); DebugValueTypeName(this); return a; }
		//decltype(auto)  operator[](int) { DebugValue(" mp []"); DebugValueTypeName(*this);    return AdjustFromThis(a); }
		//const int  operator[](int) const& { DebugValue(" const  []"); DebugValueTypeName(*this);  return a; }
	};
	test_override lv_override;
	const test_override clv_override;
	lv_override[1];
	clv_override[1];
	util::move(lv_override)[1];
	DebugValueTypeName(lv_override[1]);
	DebugValueTypeName(clv_override[1]);
	DebugValueTypeName(util::move(lv_override)[1]);

	getchar();
	return 0;
}