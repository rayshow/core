#pragma once
#include<ccdk/mpl/type_traits/remove_const.h>
#include<ccdk/mpl/type_traits/remove_volatile.h>

namespace ccdk
{
	namespace mpl
	{
		namespace tt_impl
		{
			template<typename T> struct remove_cv_helper :public remove_volatile< remove_const_t<T> > {};
		}

		//lazy
		//remove all bottom-level const/volatile qualifers
		template<typename T> struct remove_cv :public tt_impl::remove_cv_helper<T>{};
		template<typename T> using remove_cv_t = typename remove_cv<T>::type;

	}
}
