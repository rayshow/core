#include<iostream>
#include<ccdk/mpl/type_traits/has_callable.h>

using namespace ccdk::mpl;



template<typename... Args, typename FN>
void test(FN fn)
{
	std::cout << has_callable<FN, Args...>::value << std::endl;
}

void test_char(char){}
class A {};
int main() {
	
	test<char>(test_char);
	test<int>(test_char);
	test<void>(test_char);
	test<float>([](float) {});
	test<A>([](float) {});

	getchar();
	return 0;
}