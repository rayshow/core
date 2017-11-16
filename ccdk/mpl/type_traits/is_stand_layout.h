#pragma once

#include<ccdk/mpl/base/bool_.h>

namespace ccdk
{
	namespace mpl
	{
		// NOT standard layout
		//1. object member have same access control  
		//2. no virtual function
		//3. base and member object must be standard layout
		//4. only one base class own member object and derived class can't have member object
		//5. no EBO

		//can access memory like c struct
		template<typename T> struct is_standlayout :public bool_<__is_standard_layout(T)> {};
		template<typename T> constexpr bool is_standlayout_v = is_standlayout<T>::value;
	}
}