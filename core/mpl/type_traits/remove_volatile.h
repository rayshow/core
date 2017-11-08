#pragma once

namespace core
{
	namespace mpl
	{
		template<typename T> struct remove_volatile             { typedef T type; };
		template<typename T> struct remove_volatile<T volatile> { typedef T type; };
		template<typename T> using remove_volatile_t = typename remove_volatile<T>::type;
	}
}
