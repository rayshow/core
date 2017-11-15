#pragma once

#include<core/mpl/type_traits/impl/has_operator_decl.h>

namespace core
{
	namespace mpl
	{
		//test R = T::operator/(P) or R = ::operator/(T,P)
		A3D_TT_HAS_NO_SIDE_EFFECT_BINARY_OP(/, div);

		//test T::operator/=(P) or ::operator/=(T,P)
		A3D_TT_HAS_SIDE_EFFECT_BINARY_OP(/=, div_assign);
	}
}