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
#include<type_traits>


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

struct test_trivial1 { int a; };
struct test_trivial2 { 
	int a;
	test_trivial2() :a{ 0 } {};
	test_trivial2(const test_trivial2&) = default; 
};


struct test_trivial3 { test_trivial2 t;  };


int main()
{
	printf("%d", sizeof(derive));
	
#define TestHasTrivialConstructor(clazz,clazz2) printf(" %d ", std::is_trivially_assignable_v<clazz, clazz2> )
	TestHasTrivialConstructor(int, char);
	TestHasTrivialConstructor(int&, char);
	TestHasTrivialConstructor(int*, int*);
	TestHasTrivialConstructor(int*, volatile int*);
	TestHasTrivialConstructor(int*, const int*);
	TestHasTrivialConstructor(int*, const volatile int*);



	//test_trivial3 t3 = 0;

	//std::uninitialized_copy
	
	test_trivial1 t1;
	test_trivial2 t2;

	getchar();
	return 0;
}