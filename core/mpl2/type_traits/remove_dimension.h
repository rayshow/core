#pragma once

namespace Aurora3D
{
	namespace mpl
	{
		template<typename T> struct RemoveDim { typedef T type; };
		template<typename T> struct RemoveDim<T[]> { typedef T type; };
		template<typename T, size_t N> struct RemoveDim<T[N]> { typedef T type; };

		template<typename T> using RemoveDimT = typename RemoveDim<T>::type;
	}
}