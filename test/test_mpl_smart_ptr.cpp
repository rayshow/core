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

using namespace ccdk::mpl;
int main()
{
	std::shared_ptr<int> a{};
	DebugNewTitle("share_ptr")
	{
		{
			DebugSubTitle("array ");
			share_ptr<base[]> sp{ new base[5] };
			sp[1];

			//share_ptr<base> sp2{ sp };  // error
		}
		{
			DebugSubTitle("share ");
			share_ptr<derive> sp1{ new derive{} };     //derive -> derive
			share_ptr<derive> sp2{ sp1 };              //copy
			share_ptr<base>   sp3{ sp1 };              //base -> derive
			share_ptr<base[]> sp4{ new base[4] };      //base []
			//share_ptr<void>   sp5{ sp1 };            //error void -> derive 

			//share_ptr<derive[]> sp6{ sp1 };     //error   derive -> derive[]
			share_ptr<base[]> sp7{ sp4 };         //ok copy base[] -> base[]
			//share_ptr<base> sp8{ sp6 };         //error   base[] -> derive[]

		}

		{
			DebugSubTitle("void");
			share_ptr<void> sp1{ new int };
			//share_ptr<void> sp4{ new base{} };  //error void -> non-pod type
			//share_ptr<int> sp2{ sp1 };

			DebugValue(is_convertible< share_ptr<void>, share_ptr<int> >::value);

		}
	}
	
	/*share_ptr<int[]> sp2{ new int[5] };
	sp2[1];
	sp[1];*/

	

	DebugNewTitle("scope ptr");
	
	/*{
		int *p = new int;
		scope_ptr<void> vp{ new int };


		int *a = new int[2];
		DebugValue((int)p);
		DebugValue((int)&a[0]);
		DebugValue((int)&a[1]);
		base *ba = new base();
		base *bb = new derive();
		{
			scope_ptr<int> ptr{ p };
			scope_ptr<int[]> arr_ptr{ a };
			scope_ptr<base> bp1{ ba };
			scope_ptr<base> bp2{ bb };
			bp1->test_size();
			bp2->test_size();
		}
		DebugValue((int)p);
		DebugValue((int)&a[0]);
		DebugValue((int)&a[1]);
	}*/


	getchar();
	return 0;
}