#pragma once

#include<core/mpl/base/bool_.h>
#include<cstddef>

namespace core
{
	namespace mpl
	{
		template<typename T> struct is_nullptr_t :public false_ {};
		template<> struct is_nullptr_t<std::nullptr_t> :public true_ {};
		template<> struct is_nullptr_t<std::nullptr_t const> :public true_ {};
		template<> struct is_nullptr_t<std::nullptr_t volatile> :public true_ {};
		template<> struct is_nullptr_t<std::nullptr_t const volatile> :public true_ {};
		
		template<typename T> constexpr bool is_nullptr_v = is_nullptr_t<T>::value;
	}
}