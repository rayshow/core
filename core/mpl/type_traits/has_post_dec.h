#pragma once

#include<core/mpl/type_traits/impl/has_operator_decl.h>
#include<core/mpl/type_traits/declval.h>
#include<core/mpl/type_traits/is_void.h>
#include<core/mpl/type_traits/is_nullptr_t.h>
#include<core/mpl/type_traits/is_enum.h>
#include<core/mpl/type_traits/is_rref.h>
#include<core/mpl/type_traits/is_const.h>

namespace core
{
	namespace mpl
	{
		// T -- 
		// const T or T&& is ill-formed
		// enum -- is ill-formed
		// nullptr-- is ill-formed
		// void,void* is ill-formed
		namespace op_detail
		{
			template<typename T>
			struct post_dec_operation
			{
				static constexpr decltype(auto) invoke()
				{
					return makeval<T>()++;
				}
			};
			template<typename T>
			struct check_post_dec_parameter :public parameter_extracter<T>
			{
				static constexpr bool value = or_v< is_const<T>, is_rref<T>, is_enum<T>, is_nullptr_t<T>, is_void<left_noptr_t>>;
			};
		}
		template<typename T, typename Ret = null_,
			bool invalid = op_detail::check_post_dec_parameter<T>::value >
			struct has_post_dec :public op_detail::has_unary_operation< 
				op_detail::post_dec_operation<T>, T, Ret> {};

		template<typename T, typename Ret> struct has_post_dec<T, Ret, true> :public false_ {};

		template<typename T, typename Ret = null_>
		static constexpr bool has_post_dec_v = has_post_dec<T, Ret>::value;
	}
}