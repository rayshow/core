#pragma once

#include<core/mpl/type_traits/add_rref.h>
#include<core/mpl/type_traits/add_lref.h>

namespace core
{
	namespace mpl
	{
		template<typename T>  add_rref_t<T> declval() noexcept;
		template<typename T>  add_lref_t<T> makeval() noexcept;
	}
}