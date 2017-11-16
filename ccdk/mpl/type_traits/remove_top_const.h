#pragma once

namespace ccdk
{
	namespace mpl
	{
		//const T&& is rare but still list here
		template<typename T> struct remove_top_const{ typedef T  type; };
		template<typename T> struct remove_top_const<const T*> { typedef T* type; };
		template<typename T> struct remove_top_const<const T* const> { typedef T* const type; };
		template<typename T> struct remove_top_const<const T* volatile> { typedef T* volatile type; };
		template<typename T> struct remove_top_const<const T* const volatile> { typedef T* const volatile type; };
		template<typename T> struct remove_top_const<const T&> { typedef T& type; };
		template<typename T> struct remove_top_const<const T&&> { typedef T&& type; };
		template<typename T> using remove_top_const_t = typename remove_top_const<T>::type;

	}
}

