#pragma once

namespace Aurora3D
{
	namespace mpl
	{
		// reference fold
		// T   => T&
		// T&& => T&
		// T&  => T&
		// void => void
		namespace detail
		{
			template<typename T, bool IsVoid = IsVoid<T>::value> struct AddLValueRefHelper          { typedef T& type; };
			template<typename T>                                 struct AddLValueRefHelper<T, true> { typedef T  type; };
		}
		
		template<typename T> struct AddLValueRef:public detail::AddLValueRefHelper<T>{};
		template<typename T> using AddLValueRefT = typename AddLValueRef<T>::type;


	}
}

