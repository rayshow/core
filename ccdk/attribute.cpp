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
#include<ccdk/mpl/base/arg_pack.h>
#include<ccdk/mpl/base/val_pack.h>
#include<ccdk/mpl/base/integer_.h>
#include<ccdk/mpl/base/int_.h>
#include<ccdk/mpl/base/dispatch_when.h>
#include<ccdk/mpl/function/arg.h>
#include<ccdk/mpl/function/function.h>
#include<ccdk/mpl/function/bind_mfn.h>
#include<ccdk/mpl/function/partial.h>
#include<ccdk/mpl/function/capture.h>

using namespace ccdk;
using namespace ccdk::mpl;
using namespace ccdk::mpl::literals;


template<typename... Args>
auto test_tuple_storage(Args&& ... args)
{
	DebugFunctionName();
	return tuple_storage<sizeof...(Args), make_indice<sizeof...(Args)>, decay_t<Args>...>{ util::forward<Args>(args)... };
}

int test_n( test_copy_t& tt, int a) 
{
	DebugValue(a);
	return 0;
}

int test_n(test_copy_t& tt, float a)
{
	DebugValue(a);
	return 0;
}

struct test_obj
{

	int operator()(test_copy_t& tt, float a)
	{
		DebugValue(a);
		return 0;
	}

	//int operator()(test_copy_t&& tt, float a)
	//{
	//	DebugValue(a);
	//	return 0;
	//}
};

 


#include<stack>

int main()
{
	int number[30 - 2 + 1];
	for (int i = 2; i < 30; ++i)
	{
		number[i - 2] = i*i*i*i;
		DebugValue(i*i*i*i);
	}

	
	std::stack<int> st;
	for (int i = 0; i < 28; ++i)
	{
		int n = number[i];

		for (int j = 0; j < 28; ++j)
		{

		}
	}


	//debug tuple storage
	/*DebugNewTitle("test tuple storage");
	test_copy_t tc1{};
	auto tp = test_tuple_storage(util::move(tc1));
	test_copy_t tc2{ ebo_at<0>(util::move(tp))};*/

	test_obj ttt{};
	std::function<void(test_copy_t& tt, float)> ff = test_obj{};


	//AssertTrue(is_function<void(&)(void)>::value);

	//debug partial
	test_copy_t tc3{};

	typedef int(*FN)(test_copy_t&, float);



	FN pn = test_n;
	DebugValueTypeName(pn);

	//AssertTrue(is_invokable<decltype(pn)>::value);
	//AssertTrue(is_invokable<decltype(static_cast<Fn&>(pn))>::value)

	partial(pn, tc3);// (1.f);

	

	//partial(test_obj{}, tc3)(1);
	//partial(test_obj{}, util::move(tc3))(1);

	////debug capture
	//test_copy_t tc3{};
	//capture(util::move(tc3))(test_n)(1);
	//capture(tc3)(test_n)(1);

	getchar();
	return 0;
}