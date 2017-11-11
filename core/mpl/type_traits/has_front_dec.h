#pragma once

#include<core/mpl/type_traits/impl/has_operator_decl.h>
#include<core/mpl/type_traits/is_void.h>
#include<core/mpl/type_traits/is_nullptr_t.h>
#include<core/mpl/type_traits/is_enum.h>
#include<core/mpl/type_traits/is_rref.h>
#include<core/mpl/type_traits/is_const.h>
#include<core/mpl/base/or_.h>

namespace core
{
	namespace mpl
	{
		// T need to be non-const left value
		// -- T* is well formed
		// -- void* is ill-formed
		// -- void  is ill-formed
		// -- nullptr is ill-formed
		// -- enum is ill-formed
		HAS_FRONT_UNARY_OPERATION_DECL(--, front_dec, (or_v< is_const<T>, is_rref<T>, is_enum<T>, is_nullptr_t<T>, is_void<left_noptr_t>>) );
	}
}