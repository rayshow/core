#pragma once

#include<core/mpl/type_traits/impl/has_operator_decl.h>
#include<core/mpl/type_traits/impl/binary_not_match.h>

namespace core
{
	namespace mpl
	{
		HAS_BINARY_OPERATION_DECL( ||, logic_or, A3D_PP_LOGIC_NOT_MATCH(left, right));
	}
}