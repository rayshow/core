#pragma once

#include<ccdk/mpl/base/or_.h>
#include<ccdk/mpl/type_traits/is_function.h>
#include<ccdk/mpl/type_traits/is_function_obj.h>
#include<ccdk/mpl/type_traits/is_mfn_ptr.h>

namespace ccdk
{
	namespace mpl
	{
		template<typename T> struct is_invokable :or_< is_function<T>, is_mfn_ptr<T>, is_function_obj<T> > {};
	}
}

