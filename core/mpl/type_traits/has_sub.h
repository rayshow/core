#pragma once


#include<core/mpl/type_traits/impl/has_operator_decl.h>
#include<core/mpl/type_traits/impl/binary_not_match.h>
#include<core/mpl/type_traits/is_const.h>

namespace core
{
	namespace mpl
	{
		//pointer - pointer is ill-formed
		//pointer - non-integer fundemental is ill-formed
		//void*   - fundemental is ill-formed
		HAS_BINARY_OPERATION_DECL(-, sub, A3D_TT_ADD_SUB_NOT_MATCH(left_nocv_t, right_nocv_t, left_noptr_t, right_noptr_t));

		//const left is ill-formed
		HAS_BINARY_OPERATION_DECL(-=, sub_assign, A3D_TT_ADD_SUB_NOT_MATCH(left_nocv_t,
			right_nocv_t, left_noptr_t, right_noptr_t) || is_const_v<left> );
	}
}