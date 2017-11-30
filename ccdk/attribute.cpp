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
#include<ccdk/mpl/function/bind_mfn.h>
#include<ccdk/mpl/function/partial.h>

using namespace ccdk;
using namespace ccdk::mpl;
using namespace ccdk::mpl::literals;


template<typename T, typename P = decay_t<T>>
auto test(T&& t)
{
	DebugValue(&"fdas");
	DebugFunctionName();
	//return tuple_storage<1, make_indice<1>,decay_t<T>>{ t };
	return 0;
}


int main()
{
	auto a = test("fdas");

	getchar();
	return 0;
}