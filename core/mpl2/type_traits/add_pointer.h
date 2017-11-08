
#pragma once
#include<core/mpl/type_traits/remove_ref.h>

namespace Aurora3D
{
	namespace mpl
	{
		//pointer to reference ill-legal
		template<typename T> struct AddPointer { typedef  RemoveRefT<T>* type; };
		template<typename T> using AddPointerT = typename AddPointer<T>::type;
	}
}