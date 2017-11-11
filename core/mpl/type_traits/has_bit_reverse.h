#pragma once

#include<core/mpl/type_traits/impl/has_operator_decl.h>
#include<core/mpl/type_traits/is_integer.h>
#include<core/mpl/type_traits/is_fundamental.h>
#include<core/mpl/type_traits/is_pointer.h>

namespace core
{
	namespace mpl
	{
		HAS_FRONT_UNARY_OPERATION_DECL(~, bit_reverse, is_pointer_v<T> ||( is_fundamental_v<T> && !is_integer_v<T>) );
	}
}