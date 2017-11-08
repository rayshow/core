#pragma once

namespace core
{
	namespace mpl
	{
		template<typename T> struct remove_dim { typedef T type; };
		template<typename T> struct remove_dim<T[]> { typedef T type; };
		template<typename T, size_t N> struct remove_dim<T[N]> { typedef T type; };

		template<typename T> using remove_dim_t = typename remove_dim<T>::type;
	}
}