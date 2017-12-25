#include<ccdk/mpl/type_traits.h>
#include<ccdk/mpl/util/swap.h>
#include<ccdk/mpl/type_traits/result_of.h>
#include<ccdk/mpl/type_traits/args_of.h>
#include<ccdk/mpl/smart_ptr/scope_ptr.h>
#include<stdio.h>

using namespace ccdk;
using namespace ccdk::mpl;

int main()
{
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
	DebugValue(1);
	test_copy_assign_t<int> tint2{ tint };
	DebugValue(2);
	test_copy_assign_t<int> tint3{ ctint };
	DebugValue(3);
	test_copy_assign_t<short> tint4{ tint };

	getchar();
	return 0;
}