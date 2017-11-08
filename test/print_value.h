#pragma once
#include<iostream>
#include<string>
#include"print_type.h"
using namespace std;

#define STRINGIZE(v) STRINGIZE_HELPER(v)
#define STRINGIZE_HELPER(v) #v

template<typename T>
struct TypeValue
{
	void operator()()
	{
		cout << TypeNameHelper<T>{}()<<" value:"<< T::value << endl;
	}
};
