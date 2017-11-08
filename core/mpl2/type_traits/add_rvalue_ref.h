#pragma once

#include<core/mpl/type_traits/is_void.h>

namespace Aurora3D
{
	namespace mpl
	{
		// T  => T&&
		// T& => T&
		// T&&=> T&&
		//void => void

		namespace detail
		{
			template<typename T, bool IsVoid = IsVoid<T>::value> struct AddRValueRefHelper { typedef T&& type; };
			template<typename T>                                 struct AddRValueRefHelper<T, true> { typedef T type; };
		}
		template<typename T> struct AddRValueRef: public detail::AddRValueRefHelper<T>{};
		template<typename T> using AddRValueRefT = typename AddRValueRef<T>::type;
	}
}

