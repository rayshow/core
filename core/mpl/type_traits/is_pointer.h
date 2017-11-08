#pragma once

#include<core/mpl/base/bool_.h>
#include<core/mpl/type_traits/remove_cv.h>

namespace core
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
		
	}
}