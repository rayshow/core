#pragma once

#include<core/mpl/type_traits/remove_cv.h>
#include<core/mpl/type_traits/remove_ref.h>
#include<core/mpl/type_traits/remove_dimension.h>
#include<core/mpl/type_traits/add_pointer.h>
#include<core/mpl/type_traits/is_function.h>
#include<core/mpl/type_traits/is_array.h>

namespace Aurora3D
{
	namespace mpl
	{
		namespace detail
		{
			template<typename T, bool IsFunction =IsFunctionV(T), bool IsArray=IsArrayV(T)> 
								 struct DecayHelper { typedef RemoveCVT<RemoveRefT<T>> type; };
			template<typename T> struct DecayHelper<T,false, true> { typedef AddPointerT<RemoveDimT<RemoveRefT<T>>> type; };
			template<typename T> struct DecayHelper<T, true, false> { typedef AddPointerT<T> type; };
		}


		//same as a lvalue expression is used as an rvalue, cv-qualifer is stripped
		//function => function pointer : AddPointer<T>::type;
		//array    => remove dimension and convert to pointer
		//Pointer  => remove low-level const
		//lvalue/rvalue => remove reference
		//lazy 
		template<typename T> struct Decay:public detail::DecayHelper<T>{};
		template<typename T> using  Decay_t = typename Decay<T>::type;
	}
}