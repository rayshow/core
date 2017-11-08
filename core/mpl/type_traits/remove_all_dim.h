#pragma once

namespace core
{
	namespace mpl
	{
		template<typename T> struct remove_all_dim { typedef T type; };
		template<typename T> struct remove_all_dim<T[]> :public remove_all_dim<T> {};
		template<typename T, int Len> struct remove_all_dim<T[Len]> :public remove_all_dim<T> {};
		template<typename T> using remove_all_dim_t = typename remove_all_dim<T>::type;
	}
}

