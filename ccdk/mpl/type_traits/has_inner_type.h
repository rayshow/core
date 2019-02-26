#pragma once

#include<ccdk/mpl/type_traits/impl/has_nest_decl.h>
#include<ccdk/mpl/type_traits/is_base_same.h>

namespace ccdk
{
	namespace mpl
	{
		//test if typename T::type is well-formed
		CCDK_TT_HAS_NEST_TYPE_DECL(has_nest_type, type);

		//test if typename T::value_type is well-formed
		CCDK_TT_HAS_NEST_TYPE_DECL(has_nest_value_type, value_type);

		//test if typename T::tag is well-formed
		CCDK_TT_HAS_NEST_TYPE_DECL(has_nest_tag, tag);

		//test if typename T::size_type is well-formed
		CCDK_TT_HAS_NEST_TYPE_DECL(has_nest_size_type, size_type);

		//test if typename T::different_type is well-formed
		CCDK_TT_HAS_NEST_TYPE_DECL(has_nest_different_type, different_type);

		//test if typename T::categroy is well-formed
		CCDK_TT_HAS_NEST_TYPE_DECL(has_nest_category, category);

		//test if constexpr T::value is well-formed
		CCDK_TT_HAS_NEST_CONSTEXPR_DECL(has_nest_value, value);

		/* Fn::type exists, return Fn::type */
		template<typename Fn, bool = has_nest_type_v<Fn>>
		struct select_type_ : public Fn {};

		/* Fn::type not exists, return Fn::wrapper_type */
		template<typename Fn>
		struct select_type_<Fn, false> { typedef  Fn type;};

		template<typename Fn>
		using select_type_t = typename select_type_<Fn>::type;
	}
}
