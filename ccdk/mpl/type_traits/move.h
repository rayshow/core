#pragma once

#include<ccdk/mpl/type_traits/remove_ref.h>

namespace ccdk
{
	namespace mpl
	{
		template<typename T, typename P = remove_ref_t<T> >  
		inline constexpr P&& move(T inT) { return static_cast<P>(inT); }
	}
}