#pragma once

namespace Aurora3D
{
	namespace mpl
	{
		template<typename T> struct RemoveAllDim { typedef T type; };
		template<typename T> struct RemoveAllDim<T[]> :public RemoveAllDim<T> {};
		template<typename T, size_t Len> struct RemoveAllDim<T[Len]> :public RemoveAllDim<T> {};
		template<typename T> using RemoveAllDimT = typename RemoveAllDim<T>::type;
	}
}

