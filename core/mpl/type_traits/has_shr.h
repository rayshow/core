#pragma once

#include<core/mpl/type_traits/impl/has_operator_decl.h>
#include<core/mpl/type_traits/impl/binary_not_match.h>
#include<core/mpl/type_traits/is_const.h>

namespace core
{
	namespace mpl
	{
		// T* >> P* is ill-formed
		// T* >> fundamental is ill-formed
		// integer >> non-integer fundamental is ill-formed
		HAS_BINARY_OPERATION_DECL( >> , shr, A3D_PP_BIT_OP_NOT_MATCH(left_nocv_t, right_nocv_t));

		// const T >>= P is ill-formed 
		HAS_BINARY_OPERATION_DECL( >>= , shr_assign, A3D_PP_BIT_OP_NOT_MATCH(left_nocv_t, right_nocv_t) || is_const_v<left>);
	}
}