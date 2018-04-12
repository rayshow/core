
#include<stdio.h>
#include<ccdk/container/array.h>
#include<ccdk/mpl/type_traits/has_attribute.h>
#include<array>
#include<stdio.h>
#include<string>

using namespace ccdk;
using namespace ccdk::ct;

#define new_line printf("\n");

int main()
{
	DebugNewTitle("array constexpr copy ctor");
	{
		 array<int, 10> arr1{ { 1,2,3,4 } };
		 array<int, 11> arr2{ arr1 };
		 array<int, 10> arr3{ arr1 };
		arr2.debug("constexpr template copy:");
		arr2.debug("constexpr copy:");

	}
	DebugNewTitle("fill constructor");
	{
		array<int, 10> arr1{ 10, 2 };
		RuntimeAssertTrue(arr1.size() == 10);
		RuntimeAssertTrue(arr1[-1] == 2);
	}
	DebugNewTitle("move and template move constructor");
	{
		array<std::string, 10> arr1{ 1, "hello" };
		RuntimeAssertTrue(arr1.size() == 1);
		RuntimeAssertTrue(arr1[-1] == "hello");
		array<std::string, 10> arr2{ util::move(arr1) };
		RuntimeAssertTrue(arr1.size() == 0);
		array<std::string, 11> arr3{ util::move(arr2) };
	}
	DebugNewTitle("range ctor");
	{
		array<int, 10> arr1{ 10, 2 };
		array<int, 20> arr2{ arr1.begin(), arr1.size() };
		array<int, 20> arr3{ arr1.begin(), arr1.end() };
		
	}
	DebugNewTitle("assign");
	{
		array<int, 10> arr1{ 10, 2 };
		array<int, 10> arr2{ 8, 3 };
		array<int, 10> arr3{ 10, 4 };
		arr1 = { 2,3,4,5 };
		RuntimeAssertTrue(arr1[-1] == 5 && arr1[0] ==2);
		arr1 = arr2;
		RuntimeAssertTrue(arr1[-1] == 3 && arr1.size() == 8);
		arr1 = arr3;
		RuntimeAssertTrue(arr1[-1] == 4 && arr1.size() == 10);
	}
	DebugNewTitle("assign move");
	{
		array<test_copy_t, 3> arr1{ 2 };
		array<test_copy_t, 3> arr2{};
		array<test_copy_t, 4> arr3{};
		arr2 = util::move(arr1);
		arr3 = util::move(arr2);
	}
	DebugNewTitle("assign range");
	{
		array<int, 10> arr1{ 4, 2 };
		array<int, 10> arr2{ 5, 5 };
		array<int, 10> arr3{ 6,6 };
		arr1.assign(10, 4);
		RuntimeAssertTrue(arr1.size() == 10 && arr1[-1] == 4);
		arr1.assign(arr2.begin(), arr2.size());
		RuntimeAssertTrue(arr1.size() == 5 && arr1[-1] == 5);
		arr1.assign(arr3.begin(), arr3.end());
		RuntimeAssertTrue(arr1.size() == 6 && arr1[-1] == 6);
	}
	DebugNewTitle("debug iterator");
	{
		array<int, 10> arr1{ {1,2,3,4,5,6,7} };
		const array<int, 10> arr2{ { 1,2,3,4,5,6,7 } };
		RuntimeAssertTrue(arr1.size() == 7);
		RuntimeAssertTrue(arr1.capcity() == 10);
		arr1.rdebug("rv:");
		arr2.crdebug("crv:");
	}
	DebugNewTitle("test range index");
	{
		array<int, 10> arr1{ 4, 2 };
		arr1[{0, -1}] = [](int a) {
			static int init = 1;
			return init ++ * a;
		};   
	}
	

	_CrtDumpMemoryLeaks();
	getchar();
	return 0;
}