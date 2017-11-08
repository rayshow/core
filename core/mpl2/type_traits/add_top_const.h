#pragma once

namespace Aurora3D
{
	namespace mpl
	{
		// T   => T
		// T&  => const T&
		// T&& => T&& 
		// T*(const volatile) => const T*(const volatile)
		template<typename T> struct AddTopConst     { typedef T type; };
		template<typename T> struct AddTopConst<T*> { typedef const T* type; };
		template<typename T> struct AddTopConst<T* const> { typedef const T* const type; };
		template<typename T> struct AddTopConst<T* volatile> { typedef const T* volatile type; };
		template<typename T> struct AddTopConst<T* const volatile> { typedef const T* const volatile type; };
		template<typename T> struct AddTopConst<T&> { typedef const T& type; };
		template<typename T> using  AddTopConstT = typename AddTopConst<T>::type;
	}
}

