#pragma once

#include<core/mpl/base/bool_.h>

namespace core
{
	namespace mpl
	{
		template<typename T> struct is_pod :public bool_<__is_pod(T)> {};
		template<typename T> constexpr bool is_pod_v = is_pod<T>::value;
	}
}