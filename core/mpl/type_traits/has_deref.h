#pragma once

#include<core/mpl/type_traits/impl/has_operator_decl.h>
#include<core/mpl/type_traits/impl/binary_not_match.h>

namespace core
{
	namespace mpl
	{
		HAS_FRONT_UNARY_OPERATION_DECL(*, deref, 0);
	}
}