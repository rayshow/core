#pragma once

#include<ccdk/preprocessor/seq_foreach_tuple.h>

namespace ccdk
{
	namespace mpl
	{
		namespace op
		{
			struct index_t
			{
				template<typename T, typename U>
				constexpr decltype(auto)
					operator()(
						T&& t,
						U&& index
						) const
				{
					return t[index];
				}
			};

			//1 element invoker
			struct invoke_t
			{
				template<typename T, typename U>
				constexpr decltype(auto)
					operator()(
						T&& t,
						U&& u
						) const
				{
					return t(util::forward<U>(u));
				}


				template<
					typename T,
					typename U1,
					typename U2,
					typename... Args
				>
					decltype(auto)
					operator()(
						T&& t,
						U1&& u1,
						U2&& u2,
						Args&&... args) const
				{
					return t(
						util::forward<U1>(u1),
						util::forward<U2>(u2),
						util::forward<Args>(args)...
					);
				}

			};

			struct post_dec_t
			{
				template<typename T>
				constexpr decltype(auto)
					operator()(T&& t) const
				{
					return t--;
				}
			};

			struct post_inc_t
			{
				template<typename T>
				constexpr decltype(auto)
					operator()(T&& t) const
				{
					return t++;
				}
			};

			struct arrow_t
			{
				template<typename T>
				constexpr decltype(auto)
					__arrow_impl(false_, T&& t) const
				{
					return t.operator->();
				}

				template<typename T>
				constexpr decltype(auto)
					__arrow_impl(true_, T&& t) const
				{
					return t;
				}

				template<typename T>
				constexpr decltype(auto)
					operator()(T&& t) const
				{
					return __arrow_impl(
						typename is_pointer<remove_ref_t<T>>::type{},
						util::forward<T>(t)
					);
				}
			};

			
			constexpr post_dec_t post_dec{};
			constexpr post_inc_t post_inc{};
			constexpr index_t    index{};
			constexpr arrow_t    arrow{};
			constexpr invoke_t   invoke{};
			

#define CCDK_DEFINE_BINARY_FN_OBJECT(name, op,...) struct name ## _t { template<typename T1, typename T2> constexpr decltype(auto) operator()(T1&& t1, T2&& t2) const noexcept { return t1 op t2; } };  constexpr name ## _t name{};
#define CCDK_DEFINE_UNIARY_FRONT_FN_OBJECT(name, op,...) struct name ## _t { template<typename T> constexpr decltype(auto) operator()(T&& t) const noexcept { return op t; } }; constexpr name ## _t name{};

			CCDK_PP_FOREACH_TUPLE(CCDK_DEFINE_BINARY_FN_OBJECT, (
				(add, +),
				(sub, -),
				(div, / ),
				(mul, *),
				(mod, %),
				(shl, << ),
				(shr, >> ),
				(logic_and, &&),
				(logic_or , || ),
				(bit_xor, ^),
				(bit_and, &),
				(bit_or, | ),
				(add_assign, +=),
				(sub_assign, -=),
				(mul_assign, *=),
				(div_assign, /=),
				(mod_assign, %=),
				(shl_assign, <<=),
				(shr_assign, >>=),
				(bit_xor_assign, ^=),
				(bit_and_assign, &=),
				(bit_or_assign, |=),
				(less, <),
				(greater, >),
				(lequal, <=),
				(gequal, >=),
				(equal, ==),
				(nequal,!=),
				(assign, = )));

			CCDK_PP_FOREACH_TUPLE(CCDK_DEFINE_UNIARY_FRONT_FN_OBJECT, (
				(inc, ++),
				(dec, --),
				(deref, *),
				(negative, -),
				(positive, +),
				(logic_not, !),
				(bit_reverse, ~)));



#undef CCDK_DEFINE_BINARY_FN_OBJECT
#undef CCDK_DEFINE_UNIARY_FRONT_FN_OBJECT
			
		}
	}
}