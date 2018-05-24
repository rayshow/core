#include<iostream>

template<typename Char, unsigned int N>
void test_char(Char const (&arr)[N]) {
	std::cout << "test literal" << std::endl;
}

template<typename Char>
void test_char(Char const* str) {
	std::cout << "test string" << std::endl;
}

int main() {
	const char* str = "hello";
	test_char(str);
	test_char("fdsa");
	getchar();
	return 0;
}