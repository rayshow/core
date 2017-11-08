#pragma once

namespace Aurora3D
{
	namespace mpl
	{
		template<typename T> struct RemoveRef      { typedef T type; };
		template<typename T> struct RemoveRef<T&>  { typedef T type; };
		template<typename T> struct RemoveRef<T&&> { typedef T type; };
		template<typename T> using RemoveRefT = typename RemoveRef<T>::type;
	}
}
