#pragma once

#include<ccdk/mpl/type_traits/remove_cv.h>
#include<ccdk/mpl/type_traits/remove_ref.h>
#include<ccdk/mpl/type_traits/remove_dim.h>
#include<ccdk/mpl/type_traits/add_pointer.h>
#include<ccdk/mpl/type_traits/is_function.h>
#include<ccdk/mpl/type_traits/is_array.h>

namespace ccdk
{
	namespace mpl
	{
		namespace tt_detail
		{
			template<typename T, bool isFn = is_function_v<T> && !is_pointer_v<T>, bool isArray = is_array_v<T> > 
								 struct decay_helper { typedef remove_cv_t<remove_ref_t<T>> type; };
			template<typename T> struct decay_helper<T, false, true> { typedef add_pointer_t< remove_dim_t <T>> type; };
			template<typename T> struct decay_helper<T, true, false> { typedef add_pointer_t<T> type; };
		}


		//same as a lvalue expression is used as an rvalue, cv-qualifer is stripped
		//function => function pointer : AddPointer<T>::type;
		//array    => remove dimension and convert to pointer
		//Pointer  => remove low-level const
		//lvalue/rvalue => remove reference
		//lazy 
		template<typename T> struct decay:public tt_detail::decay_helper<T>{};
		template<typename T> struct decay<T&> : decay<T> {};
		template<typename T> struct decay<T&&> : decay<T> {};

		template<typename T> using  decay_t = typename decay<T>::type;

	}
}