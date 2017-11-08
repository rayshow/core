#pragma once

namespace Aurora3D
{
	namespace mpl
	{
		template<typename T> struct RemoveConst          { typedef T type; };
		template<typename T> struct RemoveConst<const T> { typedef T type; };
		template<typename T> using RemoveConstT = typename RemoveConst<T>::type;
	}
}

