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

template<typename Ret>
Ret add(Ret a, Ret b) {
	return a + b;
}

int main()
{
	std::size_t a = -2;
	std::ptrdiff_t b = -1;
	bool d = a < b;
	std::size_t c = a + b;

	DebugTypeName<decltype(a + b)>();
	DebugValue(add<std::ptrdiff_t>(a,b));

	getchar();
	return 0;
}