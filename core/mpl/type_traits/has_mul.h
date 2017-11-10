#pragma once

#include<core/mpl/type_traits/impl/has_operator_decl.h>
#include<core/mpl/type_traits/impl/binary_not_match.h>
#include<core/mpl/type_traits/is_const.h>

namespace core
{
	namespace mpl
	{
		//pointer * pointer is ill-formed
		//pointer * fundemental is ill-formed
		HAS_BINARY_OPERATION_DECL(*, mul, A3D_TT_MUL_DIV_MOD_NOT_MATCH(left_nocv_t, right_nocv_t));

		HAS_BINARY_OPERATION_DECL(*=, mul_assign, A3D_TT_MUL_DIV_MOD_NOT_MATCH(left_nocv_t, right_nocv_t) || is_const_v<left>);
	}
}