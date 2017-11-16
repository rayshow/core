#pragma once

#include<ccdk/mpl/type_traits/add_rref.h>
#include<ccdk/mpl/type_traits/add_lref.h>

namespace ccdk
{
	namespace mpl
	{
		template<typename T> add_rref_t<T> declval() noexcept;
		template<typename T> add_lref_t<T> makeval() noexcept;
	}
}