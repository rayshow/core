#pragma once

namespace Aurora3D
{
	namespace mpl
	{
		template<typename T> struct RemovePointer { typedef T type; };
		template<typename T> struct RemovePointer<T*> { typedef T type; };
		template<typename T> struct RemovePointer<T* const> { typedef T type; };
		template<typename T> struct RemovePointer<T* volatile> { typedef T type; };
		template<typename T> struct RemovePointer<T* const volatile> { typedef T type; };

		template<typename T> using RemovePointerT = typename RemovePointer<T>::type;

	}
}
