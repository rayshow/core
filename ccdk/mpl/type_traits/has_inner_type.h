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

		//test if typename T::size_type is well-formed
		CCDK_TT_HAS_INNER_DECL(size_type, size_type);

		//test if typename T::different_type is well-formed
		CCDK_TT_HAS_INNER_DECL(different_type, different_type);

		/* Fn::type exists, return Fn::type */
		template<typename Fn, bool = has_inner_type_v<Fn>>
		struct select_type_ : public Fn {};

		/* Fn::type not exists, return Fn::wrapper_type */
		template<typename Fn>
		struct select_type_<Fn, false>
		{
			typedef  Fn type;
		};

		template<typename Fn>
		using select_type_t = typename select_type_<Fn>::type;


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