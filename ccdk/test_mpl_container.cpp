#include<cstdio>
#include<utility>
#include<memory>
#include<string>
#include<typeinfo>
#include<iostream>
#include<type_traits>
#include<ccdk/mpl/type_traits.h>
#include<ccdk/mpl/base/index_sequence.h>

using namespace ccdk::mpl;

template<typename T, typename=void> struct test_when :test_when<T, when > {};
template<typename T, bool condi>    struct test_when<T, match_default<condi> >{ static constexpr int value = 1; };
template<typename T> struct test_when<T, match< is_void<T>> > { static constexpr int value = 2; };
template<typename T> struct test_when<T, match_one< is_float<T>, is_integer<T> >> { static constexpr int value = 3; };
template<typename T> struct test_when<T, match_both< is_top_const<T>, is_pointer<T> >> { static constexpr int value = 4;};
template<typename T> static constexpr int test_when_v = test_when<T>::value;

template<typename T>
inline constexpr void TestType(T inT)
{
	
	std::cout << TypeNameHelper<T>{}() << std::endl;
}

inline  void TestType2(const int& inn)
{
	std::cout << "const int&" << std::endl;
}

inline  void TestType2(int&& inn)
{
	std::cout << "int&&" << std::endl;
}

class TestForward
{
public:
	TestForward() = default;
	TestForward(const TestForward&)
	{
		std::cout << "copy" << std::endl;
	}

	TestForward(TestForward&&)
	{
		std::cout << "move" << std::endl;
	}
};

template<typename K,typename V>
struct pair :V{};

template<typename K, typename V>
inline constexpr V & pair_value(pair<K, V> & p)
{
	return p;
}

template<typename K, typename V>
inline constexpr V&& pair_value(pair<K, V>&& p)
{
	return forward<V&&>(p);
}

struct TestIndexDerive :pair<int_<0>, int_<10>>, pair<int_<1>, int_<11>> {};

int main()
{
	TestIndexDerive dd;
	TestType(pair_value<int_<1>>(dd));

	
	TestType(make_index_sequence<8>{});

	std::cout << "test forward lvalue" << std::endl;
	TestForward a{};
	TestForward b{ forward<TestForward&>(a) };
	std::cout << "test forward rvalue" << std::endl;
	TestForward c{ forward<TestForward&&>(a) };
	std::cout << "test static_cast lvalue" << std::endl;
	TestForward d{ static_cast<TestForward&>(a) };
	std::cout << "test static_cast rvalue" << std::endl;
	TestForward e{ static_cast<TestForward&&>(a) };
	std::cout << std::endl;

	std::forward<int>(0);
	TestType(0);
	int aa = 0;
	TestType(aa);
	TestType2(0);
	TestType2(aa);

	//test when
	static_assert(test_when_v<int> == 3, "");
	static_assert(test_when_v<float> == 3, "");
	static_assert(test_when_v<void> == 2, "");
	static_assert(test_when_v<const int*> == 4, "");
	static_assert(test_when_v<nullptr_t> == 1, "");
	using T = decltype(static_cast<false_>(declval<false_>()));
	std::cout << typeid(T).name() << std::endl;
	
	getchar();
	return 0;
}