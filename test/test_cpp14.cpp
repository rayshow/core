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
	char* mem = static_cast<char*>(::operator new(8));
	//::operator delete(mem, 4);
	int64 size = *(int*)(mem - 8);
	::operator delete(mem , 4);
	getchar();
	return 0;
}