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


struct empty1 {};
struct empty2 {};
struct empty3 {};
struct empty4 {};
struct empty5 {};
struct empty6 {};
 
struct derive :public empty1, public empty2, public empty3, public empty4, public empty5, public empty6
{
	int a;
};


int main()
{
	printf("%d", sizeof(derive));

	getchar();
	return 0;
}