#pragma once

#include<ccdk/mpl/base/bool_.h>
#include<ccdk/mpl/base/or_.h>
#include<ccdk/mpl/type_traits/is_fundamental.h>
#include<ccdk/mpl/type_traits/is_pointer.h>
#include<ccdk/mpl/type_traits/is_member_ptr.h>
#include<ccdk/mpl/type_traits/is_enum.h>
namespace ccdk
{
	namespace mpl
	{
		template<typename T> struct is_scaler :
			public or_< is_fundamental<T>, is_pointer<T>, is_member_ptr<T>, is_enum<T> > {};

		template<typename T> constexpr bool is_scaler_v = is_scaler<T>::value;

	}
} 