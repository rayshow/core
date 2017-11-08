#pragma once

#include<core/mpl/type_traits/has_operator_decl.h>
#include<core/mpl/type_traits/binary_not_match.h>

namespace Aurora3D
{
	namespace mpl
	{
		HAS_FRONT_UNARY_OPERATION_DECL(--, FrontDec, 0);
	}
}