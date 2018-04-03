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

uint8 mask = 0b00000001;
void test_shift()
{
	mask = ( mask << 1) | ((0b10000000 & mask) >> 7);
	0b00001000;
	DebugValue((uint32)mask);
}

int main()
{
	for (int i = 0; i < 16; ++i) test_shift();
	return 0;
}