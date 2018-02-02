#pragma once

#include<ccdk/mpl/base/type_.h>
#include<ccdk/mpl/type_traits/remove_cv.h>
#include<ccdk/mpl/type_traits/traits_case.h>

namespace ccdk
{
	namespace mpl
	{
		namespace detail
		{
			template<typename T> struct is_pointer_helper :public false_ {};
			template<typename T> struct is_pointer_helper<T*> :public true_ {};
		}

		//for non-member-pointer
		template<typename T> struct is_pointer :
			public detail::is_pointer_helper< remove_cv_t<T> > {};
		
		template<typename T> constexpr bool is_pointer_v = is_pointer<T>::value;

		CCDK_TT_SINGLE_RANK(is_pointer);
		
	}
}