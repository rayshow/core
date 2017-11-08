#pragma once

#include<core/mpl/type_traits/is_void.h>
#include<core/mpl/type_traits/add_top_const.h>
#include<core/mpl/type_traits/add_lvalue_ref.h>
#include<core/mpl/type_traits/remove_pointer.h>

namespace Aurora3D
{
	namespace mpl
	{
		//void => void
		//NonMemberFunction => NonMemberFunction
		//T    => const T&
		//T&   => const T&
		//T&&  => const T&
		//T*   => const T&
		namespace detail
		{ 
			template<typename T, bool IsVoid = IsVoidV(T) > struct ConvertConstLRefHelper { typedef AddTopConstT<AddLValueRefT<RemovePointerT<T>>> type; };
			template<typename T> struct ConvertConstLRefHelper<T, true> { typedef void type; };
		}

		//lazy force convert to const T& form
		template<typename T> struct ConvertConstLRef :public detail::ConvertConstLRefHelper<T> {};
		template<typename T> using ConvertConstLRefT = typename ConvertConstLRef<T>::type;
	}
}