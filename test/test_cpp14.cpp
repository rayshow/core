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

#include<ccdk/mpl/type_traits/has_constructor.h>
#include<ccdk/mpl/type_traits/has_assigner.h>

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

template<typename clazz, typename clazz2>
void TestHasTrivialConstructor()
{
	printf(" %d %d reverse: %d %d, trival: %d %d ", 
		std::is_trivially_assignable_v<clazz&, clazz2&>,
		std::is_trivially_constructible_v<clazz, clazz2>,
		std::is_trivially_assignable_v<clazz2&, clazz&>,
		std::is_trivially_constructible_v<clazz2, clazz>,
		std::is_trivial_v<clazz>,
		std::is_trivial_v<clazz2>);
	using namespace ccdk::mpl;
	using namespace ccdk::mpl;
	printf("==  %d %d reverse: %d %d, trival: %d %d\n",
		has_constructor_v<clazz, clazz2>,
		has_constructor_v<clazz2, clazz>,
		has_trivial_assigner_v<clazz&, clazz2&>,
		has_trivial_constructor_v<clazz, clazz2>,
		has_trivial_assigner_v<clazz2&, clazz&>,
		has_trivial_constructor_v<clazz2, clazz>,
		std::is_trivial_v<clazz>,
		std::is_trivial_v<clazz2>);
}

template<typename T, typename T2 = T::category>
struct always_valid {};

namespace it_impl {
	template<typename T, typename T2 = always_valid< T,void > >
	struct is_iterator_impl:public std::false_type{};

	template<typename T>
	struct is_iterator_impl<T, always_valid< T,typename T::category > > :public std::true_type {};
}

template<typename T>
struct is_iterator :it_impl::is_iterator_impl<T> {};

struct tsfinae { typedef int category; };

int main()
{
	//printf("%d", sizeof(derive));

	printf(" %d ", is_iterator<tsfinae>::value);

	std::vector<int>;
	
	
	//TestHasTrivialConstructor<int, int>();
	//TestHasTrivialConstructor<int, char>();
	//TestHasTrivialConstructor<int*, int*>();
	//TestHasTrivialConstructor<int*, volatile int*>();
	//TestHasTrivialConstructor<int*, int volatile *>();
	//TestHasTrivialConstructor<int volatile *, int*>();
	//TestHasTrivialConstructor<volatile int*, int volatile *>();


	

	int a = 0;
	int *b = &a;
	volatile int*c = &a;
	const int*d = &a;
	const volatile int* e = &a;


	std::vector<int> int1{};
	std::vector<int> int2(int1.begin(), int1.end());



	//test_trivial3 t3 = 0;

	//std::uninitialized_copy
	
	test_trivial1 t1;
	test_trivial2 t2;

	getchar();
	return 0;
}