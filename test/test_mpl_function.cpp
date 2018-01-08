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
#include<ccdk/mpl/mcontainer/arg_pack.h>
#include<ccdk/mpl/mcontainer/val_pack.h>
#include<ccdk/mpl/base/integer_.h>
#include<ccdk/mpl/base/integer_.h>
#include<ccdk/mpl/function/arg.h>
#include<ccdk/mpl/function/function.h>
#include<ccdk/mpl/function/bind_mfn.h>
#include<ccdk/mpl/function/partial.h>
#include<ccdk/mpl/function/capture.h>
#include<ccdk/mpl/function/combine.h>
#include<ccdk/mpl/function/dispatch.h>
#include<ccdk/mpl/function/fmap.h>
#include<ccdk/mpl/function/overload.h>
#include<ccdk/mpl/function/select_overload.h>
#include<ccdk/mpl/type_traits/select_case.h>
#include<ccdk/mpl/function/val.h>
#include<ccdk/mpl/function/expr.h>
#include<ccdk/mpl/function/ref.h>


using namespace ccdk;
using namespace ccdk::mpl;
using namespace ccdk::mpl::fn;
using namespace ccdk::mpl::literals;
using namespace ccdk::mpl::fn::ph;


template<typename T>
void fn_impl(case_t< is_function >, T&& t)
{
	DebugValue("is function");
}

template<typename T>
void fn_impl(case_t< is_function_ptr>, T&& t)
{
	DebugValue("is function pointer");
}

template<typename T>
void fn_impl(case_t< is_function_obj>, T&& t)
{
	DebugValue("is function obj");
}

template<typename T>
void fn_impl(case_t< is_mfn_ptr>, T&&t)
{
	DebugValue("is member function");
}

template<typename T>
void fn_impl(default_t, T&& t)
{
	DebugValue("default function ");
}

template<typename T>
void fn_impl(case_or<is_float, is_integer >, T&& t)
{
	DebugValue("is integer/float ");
}

template<typename T>
void fn_impl(case_and<is_pointer, is_void >, T&& t)
{
	DebugValue("is void* ");
}

template<typename T, typename P = remove_ref_t<T>>
void fn_select_test(T&& t)
{
	::fn_impl(
		select_case<
			is_function<P>,
			is_function_ptr<P>,
			is_function_obj<P>,
			is_mfn_ptr<P>,
			or_<is_float<P>, is_integer<P>>,
			and_< is_pointer<P>, is_void<remove_pointer_t<P>>>
		>,
		util::forward<T>(t)
	);
}

void simple_nfn() {}

struct test_obj
{
	int operator()(test_copy_t& tt, float a)
	{
		DebugValue(a);
		return 0;
	}
};


template<typename T>
void test_tmp(T&& t)
{
	T tt{ t };
	DebugTypeName<T>();
	DebugTypeName<decltype(t)>();
	DebugTypeName<decltype(util::forward<T>(t))>();
	DebugTypeName<decltype(tt)>();
}

int call1(const test_copy_t& tt, int a, const char*msg)
{
	DebugValue(msg);
	return a;
}

int call2(int a)
{
	return a;
}

int call3(int a)
{
	return a;
}


struct base
{
	virtual void test_size()
	{
		DebugValue("base");
	}
	virtual ~base()
	{
		DebugValue("base destructor");
	}
};

struct derive :public base
{
	virtual void test_size() override
	{
		DebugValue("derive");
	}

	virtual ~derive() override
	{
		DebugValue("derive destructor");
	}
};



struct change_t {
	void change() 
	{
		a = 1;
	}
	int a = 0;
};

template<typename T>
auto test_const(const T& t)
{
	return T{ const_cast<T&&>(t) };
}


void test(int a) noexcept
{
	DebugValue(a);

}

void test(int a, int b)
{
	DebugValue(b);
}



template<typename T, typename... Args>
struct is_invocable
{
	template<typename U, typename P = decltype( declval<U>()( declval<Args>()... ) )>
	static constexpr bool sfinae(int) { return true; }

	template<typename U>
	static constexpr bool sfinae(...) { return false; }

	static constexpr bool value = sfinae<T>(0);
};

struct TT
{
	auto operator[](int)
	{

	}
	auto operator()(int)
	{
		return 0;
	}

	auto operator()(int, int)
	{
		return 0;
	}

	auto invoke(int)
	{
		return 0;
	}
};

int test_inc2(int a) noexcept
{
	return a + 1;
}

template<typename T1, typename T2>
auto test_tmp(T1&& t1, T2&& t2)
{
	return t1 + t2;
}

template<uint32 acc, typename pack, typename... Args>
struct pack_val_step
{
	typedef pack type;
};

template<uint32 acc, uint32... indice, typename T, typename... Args>
struct pack_val_step< acc, indice_pack<indice...>, T, Args...>
	: pack_val_step< acc + T::value, indice_pack<indice..., acc>, Args...> {};


template<typename T>
void test_literal(const T& t)
{
	DebugTypeName<decltype(t)>();
}



void print(float a)
{
	DebugValue("normal float");
}

void print(int a)
{
	DebugValue("normal int");
}

struct test_overload
{
	void print(float a)
	{
		DebugValue("member float");
	}

	void print(int a)
	{
		DebugValue("member int");
	}
};


struct test_fn
{
	 constexpr static auto sfn = 1;

};

struct base_t
{
	virtual void print() {}
};

template<typename T>
struct derive_t
	:public base_t
{
	virtual void print() override {}
};


template<typename T>
decltype(auto) test_forward(T&& t)
{
	return std::forward<T>(t);
}

class test_2 {
public:
	void test1()
	{
		DebugTypeName<decltype(test_forward(*this))>();
		test_forward(*this);
	}

	void test1() const
	{
		DebugTypeName<decltype(test_forward(*this))>();
		test_forward(*this);
	}
};

int main()
{
	DebugTypeName<decltype(test_forward(1))>();
	test_forward(1);
	int vi = 1;
	DebugTypeName<decltype(test_forward(vi))>();
	test_forward(vi);
	const int ci = 1;
	DebugTypeName<decltype(test_forward(ci))>();
	test_forward(ci);

	test_2 ta{};
	ta.test1();
	const test_2 cta{};
	cta.test1();


	DebugNewTitle("test function static dispatch");
	fn_select_test(0);
	fn_select_test(simple_nfn);
	fn_select_test(&simple_nfn);
	fn_select_test(test_obj{});
	fn_select_test(&test_obj::operator());
	fn_select_test(1);
	fn_select_test(1.2f);
	fn_select_test((void*)0);

	DebugNewTitle("test bind member function");
	test_copy_t tt{};
	const test_copy_t ctt;
	constexpr test_constexpr_t cxtt;
	auto mfn1 = bind_mfn(&test_copy_t::test_mfn, tt);
	auto mfn2 = bind_mfn(&test_copy_t::test_mfn, ctt);
	auto mfn3 = bind_mfn(&test_copy_t::test_mfn, &tt);
	auto mfn4 = bind_mfn(&test_copy_t::test_mfn, &ctt);
	mfn1(1, "bind mfn1 ");
	mfn2(2, "bind mfn2 ");
	mfn3(3, "bind mfn3 ");
	mfn4(4, "bind mfn4 ");
	function<int(int, const char*)> fn{ mfn1 };
	fn(5, "bind mfn5");

	DebugNewTitle("test capture");
	auto cap1 = capture(test_copy_t{}, 4)(call1);
	DebugValue(cap1("hello, capture"));
	DebugValueTypeName(cap1);
	DebugValue(is_mfunction<decltype(cap1)>{});
	function<int(const char*)> fn2{ cap1 };
	DebugValue(fn2("hello, capture2"));

	DebugNewTitle("test combine");
	/* call3( call2( call1( 3, "hello") ) )  */
	auto cb1 = combine(call1, call2, call3);
	DebugValue(cb1(test_copy_t{}, 3, "hello,combine"));
	function<int(const test_copy_t&, int, const char*)> fn3{ cb1 };
	fn3(test_copy_t{}, 3, "hello,combine2");

	DebugNewTitle("dispatch");
	/* (--1) + (++2) == 3 */
	auto dp1 = dispatch(add, dec, inc);
	DebugValue(dp1(1,2));
	function<int(int, int)> dp2{ dp1 };
	DebugValue(dp2(1, 2));


	DebugNewTitle("fmap");
	/* (4+1) * (4-1) == 15 */
	auto fp1 = fmap(mul, add, sub);
	DebugValue(fp1(4, 1));
	function<int(int, int)> fp2{ fp1 };
	DebugValue(fp2(4, 1));

	DebugNewTitle("overload");
	overload<void(float), void(int)> ofn1(print, print);
	function<void(int)> ofn2{ ofn1 };
	function<void(float)> ofn3{ ofn1 };
	ofn1(1);
	ofn1(2.0f);
	ofn2(1);
	ofn3(1.5f);


	DebugNewTitle("select overload");
	auto sfn1 = select_overload<void(int)>(print);
	sfn1(1); /* select int ones */
	function<void(int)> sfn2{ sfn1 };
	sfn2(1);
	test_overload * to1 = new test_overload{};
	auto sfn3 = select_overload<void(int)>( to1, &test_overload::print);
	sfn3(1); /* select int ones */


	DebugNewTitle("test partial");
	DebugSubTitle("function object");
	partial(tt, 1)(" hello,partial copy object");
	function<int(const char*)> fnp{ partial(tt,1) };
	fnp("function, partial");
	partial(util::move(tt), 1)(" hello,partial move object");
	partial(ctt, 1)(" hello,partial move object");

	DebugSubTitle("function/function pointer");
	partial(test_normal_copy_function, util::move(tt))("hello,partial function");
	partial(&test_normal_copy_function, util::move(tt))("hello,partial function");
	partial(test_normal_copy_function, tt)("hello,partial function");
	partial(&test_normal_copy_function, tt)("hello,partial function");
	DebugSubTitle("member function pointer");
	partial(&test_copy_t::test_mfn, tt);// (2, "hello, 2 partial  obj member function");
	//partial(&test_copy_t::test_mfn, &tt)(2, "hello, 2 partial pointer member function");
	//partial(&test_copy_t::test_mfn, tt)(2)("hello, 3 partial  obj member function");


	//DebugNewTitle("test expr");
	//auto ph1 = _ + 2 + _ + 4 + _ + 6 + _ + 8; /*1 + 2 + 3 + 4 + 5 + 6 + 7 + 8 == 36*/
	//auto ph2 = 1_ + 1 + 2_ + _ + _ + _ + 4_;  /*1 + 2 + 3 + 2 + 3 + 4 + 5 ==  20 */
	//RuntimeAssertTrue(ph1(1, 3, 5, 7)==36);
	//RuntimeAssertTrue(ph2(2, 3, 4, 5)==20);
	//function<int(int, int, int, int)> ph3{ ph1 };
	//RuntimeAssertTrue(ph3(1, 3, 5, 7) == 36);

	//DebugSubTitle("test assign expr");
	//auto assign1 = ( _ = 1 );
	//int a = 0;
	//DebugValue(assign1(a));
	//DebugValue(a);
	
	/*DebugSubTitle("test invoke");
	auto inv = _ + _;*/
	//auto invoke1 = _(lazy_, _);// +_ + _;// +1_(lazy_, _);
	
	
	//DebugTypeName<decltype(invoke1)>();
	//DebugValue(invoke1.wild_size);
	//DebugValue(invoke1.index_size);
	//DebugValue(invoke1([](int i) { return i; }, 5, 3,2));

	DebugSubTitle("test val");
	
	/*test_copy_t test1{ 1 };
	const test_copy_t test2{ 1 };
	auto val1 = val(util::move(test1));
	auto val2 = val(test1);
	auto val3 = val(test2);
	auto val4 = val(1);
	RuntimeAssertTrue(val1().v == 1);
	RuntimeAssertTrue(val2().v == 1);
	RuntimeAssertTrue(val3().v == 1);
	RuntimeAssertTrue(val4() == 1);
	DebugTypeName< decltype(val1)::value_type >();
	DebugTypeName< decltype(val2)::value_type >();
	DebugTypeName< decltype(val3)::value_type >();
	DebugTypeName< decltype(val4)::value_type >();*/

	//DebugSubTitle("test ref");
	//int inta = 3;
	//const int intb = 4;
	//auto ref1 = ref(test1);
	//auto ref2 = ref(test2);
	//auto ref3 = ref(inta);
	//auto ref4 = ref(intb);
	//RuntimeAssertTrue(ref3() == 3);
	//RuntimeAssertTrue(ref4() == 4);
	//ref3() = 0;
	//// ref4() = 0;  //error
	//RuntimeAssertTrue(inta == 0);

	//DebugTypeName< decltype(ref1)::value_type >();
	//DebugTypeName< decltype(ref2)::value_type >();
	//DebugTypeName< decltype(ref3)::value_type >();
	//DebugTypeName< decltype(ref4)::value_type >();

	//DebugSubTitle("test cref");
	//auto cref1 = cref(test1);
	//auto cref2 = cref(test2);
	//auto cref3 = cref(inta);
	//auto cref4 = cref(intb);
	//DebugTypeName< decltype(cref1)::value_type >();
	//DebugTypeName< decltype(cref2)::value_type >();
	//DebugValue(cref3());

	getchar();
	return 0;
}