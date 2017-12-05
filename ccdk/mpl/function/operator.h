#pragma once

#include<ccdk/preprocessor/seq_foreach_tuple.h>

namespace ccdk
{
	namespace mpl
	{
		namespace op
		{
#define CCDK_DEFINE_BINARY_FN_OBJECT(name, op) struct name ## _t { template<typename T1, typename T2> constexpr decltype(auto) operator()(T1&& t1, T2&& t2) const noexcept { return t1 op t2; } };  constexpr name ## _t name{};
#define CCDK_DEFINE_UNIARY_FRONT_FN_OBJECT(name, op) struct name ## _t { template<typename T> constexpr decltype(auto) operator()(T&& t) const noexcept { return op t; } };

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
				(and_assign, && = ),
				(or_assign, || = ),
				(bit_xor_assign, ^=),
				(bit_and_assign, &=),
				(bit_or_assign, |=),
				(less, <),
				(greater, >),
				(lequal, <=),
				(gequal, >=),
				(equal, ==),
				(nequal,!=)));

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