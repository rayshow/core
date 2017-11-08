#pragma once

#include<core/mpl/base/bool_.h>
#include<core/mpl/base/or_.h>
#include<core/mpl/type_traits/is_fundamental.h>
#include<core/mpl/type_traits/is_pointer.h>
#include<core/mpl/type_traits/is_member_ptr.h>
#include<core/mpl/type_traits/is_enum.h>
namespace core
{
	namespace mpl
	{
		template<typename T> struct is_scaler :
			public or_< is_fundamental<T>, is_pointer<T>, is_member_ptr<T>, is_enum<T> > {};

		template<typename T> constexpr bool is_scaler_v = is_scaler<T>::value;

	}
} 