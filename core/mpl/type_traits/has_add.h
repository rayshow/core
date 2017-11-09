#pragma once

#include<core/mpl/type_traits/impl/has_operator_decl.h>
#include<core/mpl/type_traits/impl/binary_not_match.h>

namespace core
{
	namespace mpl
	{
		HAS_BINARY_OPERATION_DECL(+, add, BinaryNotMatchAddSubV(left_nocv_t, right_nocv_t));
	} 
}