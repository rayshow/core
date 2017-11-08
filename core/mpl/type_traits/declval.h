#pragma once

#include<core/mpl/type_traits/add_rref.h>
#include<core/mpl/type_traits/add_lref.h>

namespace Aurora3D
{
	namespace mpl
	{

		template<typename T> typename AddRValueRef<T>::type Declval();
		template<typename T> typename AddLValueRef<T>::type Make();
	}
}