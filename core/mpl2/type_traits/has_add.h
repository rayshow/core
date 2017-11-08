#pragma once

#include<core/mpl/type_traits/has_operator_decl.h>
#include<core/mpl/type_traits/binary_not_match.h>

namespace Aurora3D
{
	namespace mpl
	{
		HAS_BINARY_OPERATION_DECL(+, Add, BinaryNotMatchAddSub_v(left_nocv_t, right_nocv_t));
	} 
}