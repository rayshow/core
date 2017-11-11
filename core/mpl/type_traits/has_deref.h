#pragma once

#include<core/mpl/type_traits/impl/has_operator_decl.h>
#include<core/mpl/type_traits/impl/binary_not_match.h>
#include<core/mpl/type_traits/is_void.h>
#include<core/mpl/type_traits/is_fundamental.h>
#include<core/mpl/type_traits/is_enum.h>
#include<core/mpl/type_traits/is_member_ptr.h>


namespace core
{
	namespace mpl
	{
		// deref void* is ill-formed
		// deref fundamental is ill-formed
		// deref enum is ill-formed
		// deref obj-member-ptr is ill-formed
		// 
		HAS_FRONT_UNARY_OPERATION_DECL(*, deref, (is_void_v<T> && is_void_v<left_noptr_t>) || is_fundamental_v<T> || is_enum_v<T> || is_member_ptr_v<T> );
	}
}