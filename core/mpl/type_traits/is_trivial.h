#pragma once

#include<core/mpl/base/bool_.h>

namespace core
{
	namespace mpl
	{
		// Is Trivial
		//1. every copy/move constructor or copy/move assignment is trivial or deleted(default constructor is trivial)
		//2. at least one copy/move constructor ¡¢copy/move assignment is't deleted
		//3. trivial destructor(not virtual,not user-provided(implicitly or explicitly defaulted))
		template<typename T> struct is_trivial :public bool_<__is_trivial(T)> {};
		template<typename T> constexpr bool is_trivial_v = is_trivial<T>::value;
	}
}