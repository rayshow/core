#pragma once

namespace core
{
	namespace mpl
	{
		// T   => T
		// T&  => const T&    ref object content can't be change
		// T&& => const T&& 
		// T*(const volatile) => const T*(const volatile)
		template<typename T> struct add_top_const{ typedef T type; };
		template<typename T> struct add_top_const<T*> { typedef const T* type; };
		template<typename T> struct add_top_const<T* const> { typedef const T* const type; };
		template<typename T> struct add_top_const<T* volatile> { typedef const T* volatile type; };
		template<typename T> struct add_top_const<T* const volatile> { typedef const T* const volatile type; };
		template<typename T> struct add_top_const<T&> { typedef const T& type; };
		template<typename T> struct add_top_const<T&&> { typedef const T&& type; };
		template<typename T> using  add_top_const_t = typename add_top_const<T>::type;
	}
}

