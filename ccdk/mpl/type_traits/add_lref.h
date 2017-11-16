#pragma once

#include<ccdk/mpl/type_traits/is_void.h>

namespace ccdk
{
	namespace mpl
	{
		// reference fold
		// T   => T&
		// T&& => T&
		// T&  => T&
		// void => void
		// ref array not allowed
		// but array's ref can be implements 
		namespace detail
		{
			template<typename T, bool isVoid = is_void_v<T> > struct add_lref_helper          { typedef T& type; };
			template<typename T>                                 struct add_lref_helper<T, true> { typedef T  type; };
		}
		
		template<typename T> struct add_lref:public detail::add_lref_helper<T>{};
		template<typename T> using add_lref_t = typename add_lref<T>::type;
	}
}

