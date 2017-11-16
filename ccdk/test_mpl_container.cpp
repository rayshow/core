#include<cstdio>
#include<utility>
#include<memory>
#include<string>
#include<typeinfo>
#include<iostream>

#include<ccdk/mpl/type_traits.h>

using namespace ccdk::mpl;

template<typename T, typename=void> struct test_when :test_when<T, when > {};
template<typename T, bool condi>    struct test_when<T, match_default<condi> >{ static constexpr int value = 1; };
template<typename T> struct test_when<T, match< is_void<T>> > { static constexpr int value = 2; };
template<typename T> struct test_when<T, match_one< is_float<T>, is_integer<T> >> { static constexpr int value = 3; };
template<typename T> struct test_when<T, match_both< is_top_const<T>, is_pointer<T> >> { static constexpr int value = 4;};
template<typename T> static constexpr int test_when_v = test_when<T>::value;



int main()
{
	//test when
	static_assert(test_when_v<int> == 3, "");
	static_assert(test_when_v<float> == 3, "");
	static_assert(test_when_v<void> == 2, "");
	static_assert(test_when_v<const int*> == 4, "");
	static_assert(test_when_v<nullptr_t> == 1, "");
	using T = decltype(static_cast<false_>(declval<false_>()));
	std::cout << typeid(T).name() << std::endl;
	static_assert(is_false_<is_integer<int>>::value, "");
	getchar();
	return 0;
}