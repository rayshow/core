#pragma once

namespace ccdk
{
	namespace mpl
	{
		//T* =>  T* volatile
		//T  =>  T  volatile
		//T& =>  T&
		//T&& => T&&
		template<typename T> struct add_volatile { typedef T volatile type; };
		template<typename T> using add_volatile_t = typename add_volatile<T>::type;

	}
}

