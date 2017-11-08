#pragma once

namespace Aurora3D
{
	namespace mpl
	{
		//const T&& is rare but still list here
		template<typename T> struct RemoveTopConst           { typedef T  type; };
		template<typename T> struct RemoveTopConst<const T*> { typedef T* type; };
		template<typename T> struct RemoveTopConst<const T* const> { typedef T* const type; };
		template<typename T> struct RemoveTopConst<const T* volatile> { typedef T* volatile type; };
		template<typename T> struct RemoveTopConst<const T* const volatile> { typedef T* const volatile type; };
		template<typename T> struct RemoveTopConst<const T&> { typedef T& type; };
		template<typename T> struct RemoveTopConst<const T&&> { typedef T&& type; };
		template<typename T> using RemoveTopConstT = typename RemoveTopConst<T>::type;

	}
}

