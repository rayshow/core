#pragma once

#include<core/mpl/type_traits/impl/has_operator_decl.h>
#include<core/mpl/type_traits/is_void.h>

namespace core
{
	namespace mpl
	{
		HAS_FRONT_UNARY_OPERATION_DECL(!, logic_not, is_void_v<T>);
	}
}