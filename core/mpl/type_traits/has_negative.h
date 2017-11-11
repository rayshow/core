#pragma once

#include<core/mpl/type_traits/impl/has_operator_decl.h>
#include<core/mpl/type_traits/is_void.h>
#include<core/mpl/type_traits/is_nullptr_t.h>
#include<core/mpl/base/or_.h>

namespace core
{
	namespace mpl
	{
		// T need to be non-const left value
		// + void  is ill-formed
		// + nullptr is ill-formed
		HAS_FRONT_UNARY_OPERATION_DECL(-, negative, (or_v<is_nullptr_t<T>, is_void<T>>));
	}
}