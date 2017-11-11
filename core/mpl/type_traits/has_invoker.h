#pragma once

#include<core/mpl/type_traits/impl/has_operator_type.h>
#include<core/mpl/type_traits/impl/has_new_delete_decl.h>

namespace core
{
	namespace mpl
	{
		HAS_NEW_DELETE_DECL( operator (), invoker);
	}
}