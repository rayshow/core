#pragma once

#include<ccdk/mpl/base/integer_.h>

namespace ccdk
{
	namespace mpl
	{
		template<typename T, int dim = 0> struct array_dim:public int32_<dim> {};
		template<typename T, int len, int dim> struct array_dim<T[len], dim> : public array_dim<T, dim + 1> {};
		template<typename T, int dim> struct array_dim<T[], dim> : public array_dim<T, dim+1> {};

		template<typename T> using  array_dim_t = typename array_dim<T>::type;
		template<typename T> constexpr int array_dim_v = array_dim<T>::value;
	}

}