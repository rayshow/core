
#pragma once
#include<core/mpl/type_traits/remove_ref.h>

namespace core
{
	namespace mpl
	{
		//pointer to reference ill-legal
		template<typename T> struct add_pointer { typedef  remove_ref_t<T>* type; };
		template<typename T> using add_pointer_t = typename add_pointer<T>::type;
	}
}