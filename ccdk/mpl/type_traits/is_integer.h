#pragma once

#include<ccdk/mpl/base/bool_.h>
#include<ccdk/mpl/type_traits/is_enum.h>
#include<ccdk/mpl/type_traits/traits_case.h>

namespace ccdk
{   
	namespace mpl
	{
		template<typename T> struct is_integer :public false_ {};
		template<> struct is_integer<bool>   :public true_ {};
		template<> struct is_integer<char>  :public true_ {};
		template<> struct is_integer<wchar_t>  :public true_ {};
		template<> struct is_integer<char16_t> :public true_ {};
		template<> struct is_integer<char32_t> :public true_ {};
		template<> struct is_integer<signed char>   :public true_ {};
		template<> struct is_integer<signed short>  :public true_ {};
		template<> struct is_integer<signed int>  :public true_ {};
		template<> struct is_integer<signed long>  :public true_ {};
		template<> struct is_integer<signed long long>  :public true_ {};
		template<> struct is_integer<unsigned char> :public true_ {};
		template<> struct is_integer<unsigned short> :public true_ {};
		template<> struct is_integer<unsigned int> :public true_ {};
		template<> struct is_integer<unsigned long> :public true_ {};
		template<> struct is_integer<unsigned long long> :public true_ {};
		template<typename T> struct is_integer<T const>: public is_integer<T>{};
		template<typename T> struct is_integer<T volatile> : public is_integer<T> {};
		template<typename T> struct is_integer<T const volatile> : public is_integer<T> {};

		template<typename T> constexpr bool is_integer_v = is_integer<T>::value;

		CCDK_TT_SINGLE_RANK(is_integer);

	}
}