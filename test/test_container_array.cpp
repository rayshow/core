
#include<stdio.h>
#include<ccdk/container/array.h>
#include<array>
#include<stdio.h>
#include<string>

using namespace ccdk;
using namespace ccdk::ct;

#define new_line printf("\n");

int main()
{
	DebugNewTitle("array constexpr constructor");
	{
		constexpr array<int, 10> arr1{ { 1,2,3,4 } };
		constexpr array<int, 11> arr2{ arr1 };
		constexpr array<int, 10> arr3{ arr1 };
		for (int i = 0; i < arr2.size();++i) { printf("%d ", arr2.at(i) ); }
		new_line
		for (int i = 0; i < arr3.size(); ++i) { printf("%d ", arr3.at(i)); }
		new_line
	}
	DebugNewTitle("fill constructor");
	{
		array<int, 10> arr1{ 10, 2 };
		RuntimeAssertTrue(arr1.size() == 10);
		RuntimeAssertTrue(arr1[-1] == 2);
	}
	DebugNewTitle("move and template move constructor");
	{
		array<std::string, 10> arr1{ 10, "hello" };
		RuntimeAssertTrue(arr1.size() == 10);
		RuntimeAssertTrue(arr1[-1] == "hello");
		array<std::string, 10> arr2{ util::move(arr1) };
		RuntimeAssertTrue(arr1.size() == 0);
		array<std::string, 11> arr3{ util::move(arr2) };
	}
	DebugNewTitle("test iterator");
	{
		array<int, 10> arr1{ 4, 2 };
		arr1[{1, -1}] = [](int a) {
			static int init = 1;
			return init ++ * a;
		};   

	}

	
	getchar();
	return 0;
}