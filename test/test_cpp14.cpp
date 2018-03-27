#include<cstdio>
#include<utility>
#include<memory>
#include<string>
#include<typeinfo>
#include<iostream>
#include<type_traits>
#include<cassert>
#include<functional>
#include<thread>
#include<vector>
#include<type_traits>

#include<ccdk/type.h>
using namespace ccdk;

template<typename t> struct test_sp { constexpr static bool value = false; };

template<typename T> struct test_sp<T*>{
	constexpr static bool value = true;
};


struct test_agg {
	int a;
};

int main()
{
	int v;
	DebugValue(v = test_sp<int*>::value);
	DebugValue(v = test_sp<const int*>::value);

	//test_sp<int*>{};
	//test_sp<const int*>{};

	test_agg{ 1 };

	getchar();
	return 0;
}