#pragma once

namespace ccdk
{
	namespace mpl
	{
		template<typename T> struct remove_const          { typedef T type; };
		template<typename T> struct remove_const<const T> { typedef T type; };
		template<typename T> using remove_const_t = typename remove_const<T>::type;
	}
}

