
#include<ccdk/mpl/base/derive_if.h>
#include<ccdk/mpl/type_traits/has_inner_type.h>
#include<ccdk/memory/allocator_traits.h>
#include<memory>
#include<vector>
#include<ccdk/text/string.h>
#include<ccdk/text/string_view.h>
#include<stdio.h>

using namespace ccdk::str;
using namespace ccdk;



int main()
{
	DebugNewTitle("test string");

	DebugSubTitle("constructor ");
	string c1{ "hello" };
	DebugValue(c1.c_str());
	RuntimeAssertTrue((!strcmp(c1.c_str(), "hello")));
	string c2{ "hello", 3 };
	DebugValue(c2.c_str());
	string c3{ c1 };
	DebugValue(c3.c_str());
	string c4{ util::move(c1) };
	DebugValue(c4.c_str());
	string c5{ c4, 1,3 };
	DebugValue(c5.c_str()); 

	DebugSubTitle("swap");
	util::swap(c4, c5);

	getchar();
	return 0;
}