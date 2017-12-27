#include<cstdio>
#include<utility>
#include<memory>
#include<string>
#include<typeinfo>
#include<iostream>
#include<type_traits>
#include<cassert>
#include<functional>
#include<stdio.h>
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
#include<ccdk/mpl/smart_ptr/unique_ptr.h>
#include<ccdk/mpl/smart_ptr/poly_share_ptr.h>
#include<ccdk/mpl/smart_ptr/poly_weak_ptr.h>
#include<ccdk/mpl/smart_ptr/share_ptr.h>
#include<ccdk/mpl/smart_ptr/weak_ptr.h>
#include<ccdk/mpl/smart_ptr/smart_pointer_cast.h>

#pragma warning(disable: 4996)

using namespace ccdk;
using namespace ccdk::mpl;
using namespace ccdk::mpl::sp;

struct base
{
	virtual void test_size()
	{
		DebugValue("base test_size");
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

struct MyFileCloser
{
	void operator()(FILE* t) const 
	{
		DebugValue("enter close file");
		if (t)
		{}
	}
};


using namespace ccdk::mpl;
int main()
{
	std::shared_ptr<int> a{};
	DebugNewTitle("scope ptr");
	{
		DebugSubTitle("test normal ptr");
		scope_ptr<base> base_ptr{ new base{} };
		ccdk_assert((bool)base_ptr);

	}
	{
		DebugSubTitle("test derive ptr");
		scope_ptr<base> derive_ptr{ new derive{} };
	}
	{
		DebugSubTitle("test void ptr");
		scope_ptr<void> void_ptr{ new derive{} };
	}
	{
		DebugSubTitle("test file close");
		scope_ptr<FILE> file_ptr{ fopen("1.txt", "w"), [](FILE* f) { if (f) { DebugValue("close FILE"); fclose(f); }  } };
	}
	{
		DebugSubTitle("test scope_ptr swap");
		scope_ptr<base> base_ptr{ new base{} };
		{
			scope_ptr<derive> derive_ptr{ new derive{} };
			using namespace ccdk::mpl::util;
			swap(base_ptr, derive_ptr);
		}
	}
	{
		DebugSubTitle("test reset and release ");
		scope_ptr<base> base_ptr{ new base{} };
		base_ptr.reset();
		base_ptr.reset(new base{});
		base_ptr.reset(nullptr);
		base_ptr.reset(new derive{});
		base* ptr = base_ptr.release();
		DebugValue( (int)base_ptr.pointer());
		ptr::safe_delete(ptr);
	}
	{
		DebugSubTitle("test move");
		scope_ptr<base> base_ptr{};
		{
			scope_ptr<derive> derive_ptr{ new derive{} };
			scope_ptr<void>   void_ptr{ util::move(derive_ptr) };
			swap(void_ptr, base_ptr);
		}
		DebugValue("out of internal range");
	}
	{
		DebugSubTitle("test member and dereference");
		scope_ptr<base> base_ptr{ new base{} };
		(*base_ptr).test_size();
		base_ptr->test_size();
	} 
	{
		DebugSubTitle("test index ");
		scope_ptr<base> base_array_ptr{ new base[2], [](base* ptr) { ptr::safe_delete_array(ptr);  } };
		base_array_ptr[0].test_size();
		base_array_ptr[1].test_size();
	}



	DebugNewTitle("unique ptr");
	{
		DebugSubTitle("test normal ptr");
		unique_ptr<base> base_ptr{ new base{} };

		ccdk_assert((bool)base_ptr);
		(*base_ptr).test_size();
	}
	{
		DebugSubTitle("test derive ptr");
		unique_ptr<base> derive_ptr{ new derive{} };
	}
	{
		DebugSubTitle("test void ptr");
		unique_ptr<void> void_ptr{  };
	}
	{
		DebugSubTitle("test file close");
		struct FileClose
		{
			void operator()(FILE* file)
			{
				if (file) { DebugValue("close FILE"); fclose(file); }
			}
		};
		unique_ptr<FILE, FileClose > file_ptr{ fopen("1.txt", "w")};
	}
	{
		DebugSubTitle("test unique_ptr swap");
		unique_ptr<base> base_ptr{ new base{} };
		{
			unique_ptr<derive> derive_ptr{ new derive{} };
			using namespace ccdk::mpl::util;
			swap(base_ptr, derive_ptr);

			unique_ptr<int> int_ptr{ new int{} };
			//swap(base_ptr, int_ptr);            //error, will not found suitable fn
		}
	}
	{
		DebugSubTitle("test reset and release ");
		unique_ptr<base> base_ptr{ new base{} };
		base_ptr.reset();
		base_ptr.reset(new base{});
		base_ptr.reset(nullptr);
		base_ptr.reset(new derive{});
		base* ptr = base_ptr.release();
		DebugValue((int)base_ptr.pointer());
		ptr::safe_delete(ptr);
	}
	{
		DebugSubTitle("test move");
		unique_ptr<base> base_ptr{};
		{
			unique_ptr<derive> derive_ptr{ new derive{} };
			base_ptr = util::move(derive_ptr);

		}
		DebugValue("out of internal range");
	}
	{
		DebugSubTitle("test member and dereference");
		unique_ptr<base> base_ptr{ new base{} };
		(*base_ptr).test_size();
		base_ptr->test_size();
	}
	{
		DebugSubTitle("test index ");
		unique_ptr<base[]> base_array_ptr{ new base[2] };
		DebugTypeName<typename unique_ptr<base[]>::deleter_type>();
		base_array_ptr[0].test_size();
		base_array_ptr[1].test_size();
	}



	DebugNewTitle("poly_share_ptr");
	{
		DebugSubTitle("test normal ptr");
		poly_share_ptr<base> base_ptr1{};
		poly_share_ptr<base> base_ptr2{nullptr};
		poly_share_ptr<base> base_ptr3{ new base{} };
		poly_share_ptr<base> base_ptr4;
		DebugValue(base_ptr4.share_count());
		base_ptr4 = base_ptr3;
		DebugValue(base_ptr4.share_count());
		base_ptr4 = nullptr;
		DebugValue(base_ptr4.share_count());
	}
	{
		DebugSubTitle("test derived ptr");
		poly_share_ptr<base> base_ptr1{ new derive{} };
		poly_share_ptr<derive> derive_ptr{ new derive{} };
		poly_share_ptr<base> base_ptr2;
		DebugValue(base_ptr2.share_count());
		base_ptr2 = derive_ptr;
		derive_ptr = nullptr;
		DebugValue(base_ptr2.share_count());
		base_ptr2 = base_ptr1;
		DebugValue(base_ptr2.share_count());
		base_ptr2 = nullptr;
		DebugValue(base_ptr2.share_count());

	}
	{
		DebugSubTitle("test void ptr");
		poly_share_ptr<void> void_ptr;
		poly_share_ptr<base> base_ptr{ new base{} };
		poly_share_ptr<derive> derive_ptr{ new derive{} };
		DebugValue(void_ptr.share_count());
		void_ptr = util::move(base_ptr);
		DebugValue(void_ptr.share_count());
		void_ptr = derive_ptr;
		DebugValue(void_ptr == derive_ptr);
		DebugValue(void_ptr.share_count());
		void_ptr = nullptr;
		DebugValue(void_ptr.share_count());
	}
	{
		DebugSubTitle("test share ptr");
		poly_share_ptr<derive> sp1;
		DebugValue(sp1.share_count());
		DebugValue((bool)sp1);
		{
			poly_share_ptr<derive> sp2{ new derive{} };
			DebugValue(sp2.share_count());
			DebugValue((bool)sp2);
			sp1 = sp2;
			DebugValue(sp1.share_count());
			DebugValue((bool)sp1);
		}
		DebugValue(sp1.share_count());
		DebugValue((bool)sp1);
		sp1.reset();
		DebugValue(sp1.share_count());
		DebugValue((bool)sp1);
	}
	{
		DebugNewTitle("test copy constructor");
		poly_share_ptr<base> base1{ new base{} };
		DebugValue(base1.share_count());
		poly_share_ptr<base> base2{ base1 };
		DebugValue(base1.share_count());
		poly_share_ptr<base> base3;
		base3 = base1;
		DebugValue(base1.share_count());
	}
	
	{
		{
			DebugSubTitle("test weak ref");
			poly_weak_ptr<base> wp;
			DebugValue(wp.expired());
			DebugValue(wp.share_count());
			DebugValue(wp.weak_count());
			{
				poly_share_ptr<base> sp{ new base{} };
				wp = sp;
				DebugValue(wp.expired());
				DebugValue(wp.share_count());
				DebugValue(wp.weak_count());
				auto sp2 = wp.lock();
				DebugValue(wp.expired());
				DebugValue(wp.share_count());
				DebugValue(wp.weak_count());

				auto wp2{ wp };
				DebugValue(is_convertible<decltype(wp2), decltype(wp)>{});
				DebugTypeName<decltype(wp2)>();
				DebugValue(wp.expired());
				DebugValue(wp.share_count());
				DebugValue(wp.weak_count());
			}
			DebugValue("out of share");
			DebugValue(wp.expired());
			DebugValue(wp.share_count());
			DebugValue(wp.weak_count());
		}
		DebugValue("out of weak");
	}




	getchar();
	return 0;
}