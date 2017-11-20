#include<cstdio>
#include<utility>
#include<memory>
#include<string>
#include<typeinfo>
#include<iostream>
#include<type_traits>
#include<cassert>
#include<ccdk/mpl/type_traits.h>
#include<ccdk/mpl/base/arg_pack.h>

using namespace ccdk::mpl;


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
	DebugTypeName(T);
	DebugValueTypeName(util::forward<T>(t));
	DebugFunctionName();
	return util::forward<T>(t);
}




int main()
{
	//test arg pack
	typedef arg_pack<int, short, long, char, float> pack5;
	typedef arg_pack_ingore_index< 2,2, pack5> ingore_index;
	DebugTypeName(typename ingore_index::head_pack);
	DebugTypeName(typename ingore_index::tail_pack);


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


	//test auto
	DebugNewTitle("test auto and decltype");
	auto ta = 27;     //deduce int
	auto tb(27);      //deduce int
	auto td{ 27 };    //deduce int
	auto tc = { 27 }; //deduce initalize_list<int>
	DebugValueTypeAndValue((ta)); // decltype((var)) preduce T&
	DebugValueTypeAndValue(ta);
	DebugValueTypeAndValue(tb);
	DebugValueTypeName(tc);
	DebugValueTypeAndValue(td);

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