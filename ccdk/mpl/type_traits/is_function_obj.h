#pragma once

#include<ccdk/mpl/base/and_.h>
#include<ccdk/mpl/type_traits/is_class.h>
#include<ccdk/mpl/type_traits/has_invoker.h>

namespace ccdk
{
	namespace mpl
	{
		//a function object is a class has  operator()
		// defect : can't check override operator()
		template<typename T>
		struct is_function_obj :and_<is_class<T>, has_invoker<T>> {};

		CCDK_TT_COMPOSE_RANK(1, is_function_obj, is_class, has_invoker);

		template<typename T>
		static constexpr bool is_function_obj_v = is_function_obj<T>::value;
	}
}