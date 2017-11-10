#pragma once

#include<core/mpl/type_traits/impl/has_operator_decl.h>
#include<core/mpl/type_traits/impl/binary_not_match.h>
#include<core/mpl/type_traits/is_pointer.h>

namespace core
{
	namespace mpl
	{
		//T* >= P* (and T and P did't have derive reletion)
		//T* >= fundamental is ill-formed
		HAS_BINARY_OPERATION_DECL(>= , gequal, A3D_TT_CMP_NOT_MATCH(left_nocv_t, right_nocv_t));
	}
}