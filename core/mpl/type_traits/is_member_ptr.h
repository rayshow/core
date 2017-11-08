#pragma once

#include<core/mpl/base/bool_.h>
#include<core/mpl/type_traits/remove_cv.h>

namespace core
{
	namespace mpl
	{
		namespace detail
		{
			template<typename T> struct is_member_ptr_helper :public false_ {};
			template<typename T, typename C> struct is_member_ptr_helper<T C::*> :public true_ {};
		}
		template<typename T> struct is_member_ptr :
			public detail::is_member_ptr_helper< remove_cv_t<T> > {};

		template<typename T> constexpr bool is_member_ptr_v = is_member_ptr<T>::value;
	}
}