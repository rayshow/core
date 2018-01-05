#include<ccdk/mpl/type_traits.h>
#include<ccdk/mpl/util/swap.h>
#include<ccdk/mpl/type_traits/result_of.h>
#include<ccdk/mpl/type_traits/args_of.h>
#include<ccdk/mpl/function/function.h>
#include<ccdk/mpl/function/arg.h>
#include<ccdk/mpl/function/bind_mfn.h>
#include<ccdk/mpl/function/capture.h>
#include<ccdk/mpl/function/combine.h>
#include<ccdk/mpl/function/dispatch.h>
#include<ccdk/mpl/function/fmap.h>
#include<ccdk/mpl/function/overload.h>
#include<ccdk/mpl/function/partial.h>
#include<ccdk/mpl/function/ref.h>
#include<ccdk/mpl/function/val.h>
#include<ccdk/mpl/function/expr.h>
#include<ccdk/mpl/function/select_overload.h>
#include<ccdk/mpl/function/operator.h>
#include<stdio.h>

#include<ccdk/mpl/mcontainer/arg_pack.h>
#include<ccdk/mpl/mcontainer/val_pack.h>
#include<ccdk/mpl/mcontainer/at.h>
#include<ccdk/mpl/mcontainer/begin.h>
#include<ccdk/mpl/mcontainer/deref.h>
#include<ccdk/mpl/mcontainer/next.h>
#include<ccdk/mpl/mcontainer/merge.h>
#include<ccdk/mpl/mcontainer/find.h>

using namespace ccdk;
using namespace ccdk::mpl;


class destructor_nothrow
{
public:
	destructor_nothrow() noexcept{}
	~destructor_nothrow() noexcept {}
};

class destructor_maythrow
{
public:
	destructor_maythrow() { throw int{}; }

	~destructor_maythrow() throw(int) { throw int{}; }
};


class base_op
{
public:

	base_op operator=(const base_op& other)
	{
		DebugValue("base assign");
		return base_op{};
	}
};

class derive_op: public base_op
{
public:

	using base_op::operator=;

	derive_op operator=(const derive_op& other)
	{
		return derive_op{};
	}
};

class test_convertible
{
public:
	 test_convertible() {}
	 explicit test_convertible(const int& a) {}
	 explicit operator int() { return 0; }
};

template<typename T>
void test_template_cast(const T& t)
{}

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
using namespace ccdk::mpl::fn;


class base_class
{
public:
	base_class() {}
	base_class(base_class const& other) { DebugValue("const&"); }

	template<typename T>
	base_class(T&& t) { DebugValue("template &&"); }

};


int main()
{
	typedef arg_pack<int, char, float, short, double> apack1;
	typedef arg_pack<int[] , char[]> apack2;
	DebugTypeName< at_t<apack1, 0>>();
	DebugTypeName< at_t<apack1, 1>>();
	DebugTypeName< at_t<apack1, 2>>();
	DebugTypeName< at_t<apack1, 3>>();
	DebugTypeName< at_t<apack1, 4>>();
	DebugTypeName< merge_t< apack1, apack2>>();
	DebugTypeName< merge_t< arg_pack<>, apack2>>();
	DebugTypeName< find_at< apack1, float> >();

	typedef val_pack<int, 1, 2, 3, 5, 6, 7> vpack1;
	typedef val_pack<int, 8,9,10> vpack2;
	DebugTypeName< at_t<vpack1, 0>>();
	DebugTypeName< at_t<vpack1, 1>>();
	DebugTypeName< at_t<vpack1, 2>>();
	DebugTypeName< at_t<vpack1, 3>>();
	DebugTypeName< at_t<vpack1, 4>>();
	DebugTypeName< merge_t< vpack1, vpack2>>();
	DebugTypeName< merge_t< val_pack<int>, vpack2>>();

	const base_class  cbc{};
	base_class bc{ cbc };

	DebugNewTitle("test combine");
	/* call3( call2( call1( 3, "hello") ) )  */
	auto cb1 = combine(call1, call2, call3);/*
	DebugValue(cb1(test_copy_t{}, 3, "hello,combine"));
	function<int(const test_copy_t&, int, const char*)> fn3{ cb1 };*/

	//fn3(test_copy_t{}, 3, "hello,combine2");
	bool bv;
	DebugValue(bv = is_compatible<test_convertible, int>::value);
	DebugValue(bv = is_convertible<int, test_convertible>::value);
	
	DebugValue(bv = is_compatible<test_convertible, int>::value);
	DebugValue(bv = is_convertible<test_convertible, int>::value);
	
	DebugValue(bv = has_constructor<test_convertible, int>::value);
	DebugValue(bv = has_constructor<test_convertible, int&>::value);
	DebugValue(bv = has_constructor<test_convertible, int&&>::value);
	DebugValue(bv = has_constructor<test_convertible, const int&>::value);
	DebugValue(bv = has_constructor<int,test_convertible>::value);

	

	//test_template_cast<int>(test_convertible{});

	getchar();
	return 0;
	DebugValue("test using");
	derive_op op1{}, op2{};
	DebugTypeName<decltype(op1 = op2)>();


	DebugNewTitle("test swap");
	DebugSubTitle("test pointer swap");
	using util::swap;
	test_copy_t* p1 = 0;
	test_copy_t* p2 = (test_copy_t*)1;
	void* p3 = (void*)3;
	int*  p4 = (int*)4;
	DebugValue(p1);
	DebugValue(p2);
	DebugValue(p3);
	swap(p1, p2);
	DebugValue(p1);
	DebugValue(p2);
	DebugValue(p3);
	swap(p1, p3);
	DebugValue(p1);
	DebugValue(p2);
	DebugValue(p3);
	DebugSubTitle("swap ref");
	test_swap::test_swap_t r1{ 1 };
	test_swap::test_swap_t r2{ 2 };
	test_swap::test_swap_t2 r3{ 3 };
	test_swap::test_swap_t2 r4{ 4 };
	DebugValue(r1.value);
	DebugValue(r2.value);
	swap(r1, r2);
	DebugValue(r1.value);
	DebugValue(r2.value);
	DebugValue(r3.value);
	DebugValue(r4.value);
	swap(r3, r4);
	DebugValue(r3.value);
	DebugValue(r4.value);
	DebugSubTitle("test move");
	int v;
	DebugValue( v=has_move_assigner<test_swap::test_swap_t2>::value);
	DebugValue( v=has_move_constructor<test_swap::test_swap_t2>::value);
	DebugSubTitle("swap array");
	test_swap::test_swap_t arr1[2];
	test_swap::test_swap_t arr2[2];
	swap(arr1, arr2);
	DebugSubTitle("no suitable overload found");
	//swap(p1, p4);

	DebugNewTitle("debug test copy_t");
	test_copy_assign_t<int> tint;
	const test_copy_assign_t<int> ctint;
	test_copy_assign_t<int> tint2{};
	test_copy_assign_t<short> tint3{};
	DebugValue(1);
	test_copy_assign_t<int>   tint4{ tint };
	test_copy_assign_t<short> tint5{ tint };
	DebugValue(2);
	test_copy_assign_t<int> tint6{ ctint };
	test_copy_assign_t<short> tint7{ ctint };
	DebugValue(3);
	test_copy_assign_t<int> tint8{ util::move(tint) };
	test_copy_assign_t<short> tint9{ util::move(tint) };
	DebugValue(4);
	tint2 = tint;
	tint3 = tint;
	DebugValue(5);
	tint2 = ctint;
	tint3 = ctint;
	DebugValue(6);
	tint2 = util::move(tint);
	tint3 = util::move(tint);

	int *a = nullptr;
	ptr::safe_delete(a);
	
	DebugNewTitle("debug nothrow destructor");
	DebugValue(has_nothrow_destructor_v<destructor_nothrow>);
	DebugValue(has_nothrow_destructor_v<destructor_maythrow>);
	DebugValue(has_nothrow_constructor_v<destructor_nothrow>);
	DebugValue(has_nothrow_constructor_v<destructor_maythrow>);

	destructor_maythrow *ptr1 = nullptr;
	destructor_maythrow *ptr2 = nullptr;
	try
	{
		ptr1 = new destructor_maythrow{};
		DebugValue("maythrow 1");
	}
	catch(...){
		DebugValue(ptr1);
		DebugValue("throw 1");
	}
	try
	{
		ptr2 = new(std::nothrow_t{}) destructor_maythrow[10000000000000000ULL];
		DebugValue("maythrow 2");
	}
	catch (...) {
		DebugValue(ptr2);
		DebugValue("throw 2");
	}

	getchar();
	return 0;
}