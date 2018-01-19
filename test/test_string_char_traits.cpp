#include<cstdio>
#include<utility>
#include<memory>
#include<string>
#include<typeinfo>
#include<iostream>
#include<type_traits>
#include<cassert>
#include<functional>
#include<ccdk/string/char_traits.h>
#include<ccdk/mpl/base/integer_.h>
#include<ccdk/mpl/util/move.h>

using namespace ccdk;
using namespace ccdk::str;
using namespace ccdk::mpl;

template<typename T>
void test_copy(const T& src)
{
	T dest{};
	DebugValue(is_pod_v<T>);
	DebugValue(has_copy_assigner_v<T>);
	DebugValue(dest);
	DebugValue(src);
	util::copy(dest, src);
	DebugValue(dest);
	DebugValue(src);
}

template<typename T1, typename T2>
void test_copy(T1& dest, const T2& src)
{
	DebugFunctionName();
	DebugNewTitle("new ");
	DebugValue(is_pod_v<remove_pointer_t< remove_all_dim_t<T1>>>);
	DebugValue(has_copy_assigner_v<remove_pointer_t< remove_all_dim_t<T1>>>);
	if (is_array_v<T1>)
	{
		for (int i = 0; i < array_len_v<T1>; ++i)
		{
			std::cout << dest[i];
		}
		std::cout << "|" << std::endl;
		for (int i = 0; i < array_len_v<T2>; ++i)
		{
			std::cout << src[i];
		}
		std::cout << "|" << std::endl;
	}
	else {
		DebugValue(dest);
	}
	util::copy(dest, src);
	if (is_array_v<T1>)
	{
		for (int i = 0; i < array_len_v<T1>; ++i)
		{
			std::cout << dest[i];
		}
		std::cout << "|" << std::endl;
		for (int i = 0; i < array_len_v<T2>; ++i)
		{
			std::cout << src[i];
		}
		std::cout << "|" << std::endl;
	}
	else {
		DebugValue(dest);
	}
}

class test_copy_class
{
public:
	void operator=(const test_copy_class&) {}
};

int main()
{
	char c = 0;
	util::copy(c, 'a');
	util::move(c, 'a');

	

	std::string{};

	char a[6];
	util::copy(a, "abc");
	std::string strs1[3] = { };
	std::string strs2[2] = {"abc", "def"};
	util::copy(strs1, strs2);
	util::move(strs1, strs2);

	char buffer[20];
	const char *name = "hello,world";
	util::copy(buffer, name, strlen(name) + 1);
	util::move(buffer, name, strlen(name) + 1);

	std::string strs3[3] = {};
	std::string strs4[2] = {"hello", "world"};

	strs3[1] = util::fmove(strs4[1]);
	strs3[0] = util::fmove(strs4[0]);

	util::copy(strs3, strs4, 2);
	util::move(strs3, strs4, 2);

	test_copy_class tca;
	util::copy(tca, test_copy_class{});
	util::move(tca, test_copy_class{});

	getchar();
	return 0;
}