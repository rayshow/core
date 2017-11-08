#pragma once

#include<core/mpl/type_traits/remove_cv.h>
#include<core/mpl/type_traits/remove_ref.h>
#include<core/mpl/type_traits/remove_all_dim.h>
#include<core/mpl/type_traits/add_pointer.h>
#include<core/mpl/type_traits/is_function.h>
#include<core/mpl/type_traits/is_array.h>

namespace core
{
	namespace mpl
	{
		namespace detail
		{
			template<typename T, bool isFn =IsFunctionV(T), bool isArray =IsArrayV(T)> 
								 struct decay_helper { typedef remove_cv_t<remove_ref_t<T>> type; };
			template<typename T> struct decay_helper<T, false, true> { typedef AddPointerT<RemoveDimT<RemoveRefT<T>>> type; };
			template<typename T> struct decay_helper<T, true, false> { typedef AddPointerT<T> type; };
		}


		//same as a lvalue expression is used as an rvalue, cv-qualifer is stripped
		//function => function pointer : AddPointer<T>::type;
		//array    => remove dimension and convert to pointer
		//Pointer  => remove low-level const
		//lvalue/rvalue => remove reference
		//lazy 
		template<typename T> struct decay:public detail::decay_helper<T>{};
		template<typename T> using  decay_t = typename decay<T>::type;
	}
}