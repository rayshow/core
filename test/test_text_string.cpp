
#include<ccdk/mpl/base/derive_if.h>
#include<ccdk/mpl/type_traits/has_inner_type.h>
#include<ccdk/mpl/type_traits/has_swap.h>
#include<ccdk/memory/allocator_traits.h>
#include<memory>
#include<vector>
#include<ccdk/text/string.h>
#include<ccdk/text/string_view.h>
#include<stdio.h>

using namespace ccdk::txt;
using namespace ccdk;

struct test_swap_t
{
};

int main()
{
	DebugNewTitle("test string");

	DebugSubTitle("constructor ");
	string c1{ "hello" };
	DebugValue(c1.c_str());
	string c2{ "hello", 3 };
	DebugValue(c2.c_str());
	string c3{ c1 };
	DebugValue(c3.c_str());
	string c4{ util::move(c1) };
	DebugValue(c4.c_str());
	string c5{ c4, 1,3 };
	DebugValue(c5.c_str()); 
	string c6{ 10, 'a' };
	DebugValue(c6.c_str());

	DebugSubTitle("swap");
	string s1{ "hello" };
	string s2{ "world" };
	util::swap(s1, s2);
	DebugValue(s1.c_str());
	DebugValue(s2.c_str());
	
	
	getchar();
	return 0;
}