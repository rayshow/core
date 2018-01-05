#pragma once

#include<ccdk/mpl/base/logic_.h>
#include<ccdk/mpl/type_traits/is_function.h>
#include<ccdk/mpl/type_traits/is_function_obj.h>
#include<ccdk/mpl/type_traits/is_mfn_ptr.h>

namespace ccdk
{
	namespace mpl
	{
		// for non-overload callable type
		template<typename T> struct is_invocable :or_< is_function<T>, is_mfn_ptr<T>, is_function_obj<T> > {};
	}
}

