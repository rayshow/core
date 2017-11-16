
#pragma once
#include<ccdk/mpl/base/not_.h>
#include<ccdk/mpl/base/and_.h>
#include<ccdk/mpl/type_traits/is_member_ptr.h>
#include<ccdk/mpl/type_traits/is_mfn_ptr.h>

namespace ccdk
{
	namespace mpl
	{
		//member object pointer
		//is member pointer but not member function pointer
		template<typename T> struct is_mobj_ptr :
			public and_< is_member_ptr<T>, not_< is_mfn_ptr<T>> > {};

		template<typename T> constexpr bool is_mobj_ptr_v = is_mobj_ptr<T>::value;
	}
}