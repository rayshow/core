#pragma once

#include<core/mpl/type_traits/impl/has_operator_decl.h>
#include<core/mpl/type_traits/impl/binary_not_match.h>
#include<core/mpl/type_traits/is_pointer.h>

namespace core
{
	namespace mpl
	{
		//T*    + integer is well-formed
		//void* + integer is ill-formed
		HAS_BINARY_OPERATION_DECL(+, add, A3D_TT_ADD_SUB_NOT_MATCH(left_nocv_t, right_nocv_t));
	} 
}