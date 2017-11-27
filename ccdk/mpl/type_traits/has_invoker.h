#pragma once

#include<ccdk/mpl/type_traits/impl/has_operator_decl.h>

namespace ccdk
{
	namespace mpl
	{
		
		//class define Ret T::operator()( Args... )
		HAS_MEMBER_FN_DECL( operator (), invoker);

		namespace detail
		{
			template<typename T>
			struct has_invoker_helper<T, null_>
			{
				template<typename U, typename = decltype(&U::operator ())>
				static constexpr bool sfinae(int) { return true; }
				template<typename U>
				static constexpr bool sfinae(...) { return false; }
				static constexpr bool value = sfinae<T>(0);
			};
		}
	}
}