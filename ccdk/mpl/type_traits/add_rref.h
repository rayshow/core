#pragma once

#include<ccdk/mpl/type_traits/is_void.h>

namespace ccdk
{
	namespace mpl
	{
		// T  => T&&
		// T& => T&
		// T&&=> T&&
		//void => void
		namespace detail
		{
			template<typename T, bool IsVoid = is_void_v<T> > struct add_rref_helper { typedef T&& type; };
			template<typename T>                                 struct add_rref_helper<T, true> { typedef T type; };
		}
		template<typename T> struct add_rref: public detail::add_rref_helper<T>{};
		template<typename T> using add_rref_t = typename add_rref<T>::type;
	}
}

