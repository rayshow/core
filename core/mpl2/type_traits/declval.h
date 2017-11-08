#pragma once

#include<core/mpl/type_traits/add_rvalue_ref.h>
#include<core/mpl/type_traits/add_lvalue_ref.h>

namespace Aurora3D
{
	namespace mpl
	{

		template<typename T> typename AddRValueRef<T>::type Declval();
		template<typename T> typename AddLValueRef<T>::type Make();
	}
}