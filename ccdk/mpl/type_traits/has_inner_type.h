#pragma once

#include<ccdk/mpl/type_traits/impl/has_operator_decl.h>
#include<ccdk/mpl/type_traits/is_base_same.h>

namespace ccdk
{
	namespace mpl
	{
		//test if typename T::type is well-formed
		CCDK_TT_HAS_INNER_DECL(type, type);

		//test if typename T::value_type is well-formed
		CCDK_TT_HAS_INNER_DECL(value_type, value_type);

		//test if typename T::tag is well-formed
		CCDK_TT_HAS_INNER_DECL(tag, tag);

		template<typename T, typename V>
		struct has_inner_value
		{
			template<typename U, typename P, P p= U::value>
			constexpr static  bool sfinae(int) { return is_base_same_v<P,decltype(U::value)>; };
			template<typename U, typename P>
			constexpr static  bool  sfinae(...) { return false; };
			static constexpr bool value = sfinae<T, V>(0);
		};
		template<typename T, typename V>                                                               
		static constexpr bool has_inner_value_v = has_inner_value<T, V>::value;
	}
}