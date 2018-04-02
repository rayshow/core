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

int main()
{
	getchar();
	void* memory = ::operator new(1024 * 1024 * 1024);
	printf("after alloc");
	getchar();
	::operator delete(memory);
	getchar();

	return 0;
}