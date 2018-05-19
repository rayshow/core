#pragma once

#include<ccdk/mpl/type_traits/add_rref.h>
#include<ccdk/mpl/type_traits/add_lref.h>
#include<ccdk/mpl/type_traits/decay.h>
namespace ccdk
{
	namespace mpl
	{
		/* declare but not implements */
		template<typename T> add_rref_t<T> declval() noexcept;
		template<typename T> add_lref_t<T> makeval() noexcept;
		template<typename T> decay_t<T> decayval() noexcept;
	}
}