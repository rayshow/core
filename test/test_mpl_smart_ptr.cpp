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
#include<ccdk/mpl/function/arg.h>
#include<ccdk/mpl/function/function.h>
#include<ccdk/mpl/function/bind_mfn.h>
#include<ccdk/mpl/function/partial.h>
#include<ccdk/mpl/function/capture.h>
#include<ccdk/mpl/function/combine.h>
#include<ccdk/mpl/function/operator.h>
#include<ccdk/mpl/type_traits/select_case.h>
#include<ccdk/mpl/smart_ptr/scope_ptr.h>
#include<ccdk/mpl/smart_ptr/share_ptr.h>
#include<ccdk/mpl/smart_ptr/weak_ptr.h>

using namespace ccdk;
using namespace ccdk::mpl;
using namespace ccdk::mpl::sp;

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


void normal_deleter()
{

}

using namespace ccdk::mpl;
int main()
{
	DebugNewTitle("scope ptr");
	{
		scope_ptr<base> base_ptr{ new base{} };
	}
	

	getchar();
	return 0;
}