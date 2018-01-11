#pragma once

#include<ccdk/preprocessor/seq_foreach_tuple.h>
#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/function/function_fwd.h>

ccdk_namespace_mpl_fn_start

	struct index_t
	{
		typedef mfunction_tag tag;

		template<typename T, typename U>
		CCDK_FORCEINLINE  constexpr decltype(auto) operator()( T&& t, U&& index ) const
		{
			return t[index];
		}
	};

	//1 element invoker
	struct invoke_t
	{
		typedef mfunction_tag tag;

		template<typename T, typename...Args>
		CCDK_FORCEINLINE  constexpr decltype(auto) operator()( T&& t, Args&&... args ) const
		{
			return t(util::forward<Args>(args)...);
		}
	};

	struct post_dec_t
	{
		typedef mfunction_tag tag;

		template<typename T>
		CCDK_FORCEINLINE  constexpr decltype(auto) operator()(T&& t) const
		{
			return t--;
		}
	};

	struct post_inc_t
	{
		typedef mfunction_tag tag;

		template<typename T>
		CCDK_FORCEINLINE  constexpr decltype(auto) operator()(T&& t) const
		{
			return t++;
		}
	};

	struct arrow_t
	{
		typedef mfunction_tag tag;

		template<typename T>
		CCDK_FORCEINLINE  constexpr decltype(auto) _arrow_impl(false_, T&& t) const
		{
			return t.operator->();
		}

		template<typename T>
		CCDK_FORCEINLINE  constexpr decltype(auto) _arrow_impl(true_, T&& t) const
		{
			return t;
		}

		template<typename T>
		CCDK_FORCEINLINE  constexpr decltype(auto) operator()(T&& t) const
		{
			return _arrow_impl( typename is_pointer<remove_ref_t<T>>::type{}, util::forward<T>(t) );
		}
	};

	struct max_t
	{
		typedef mfunction_tag tag;

		template<typename T1, typename T2>
		CCDK_FORCEINLINE  constexpr decltype(auto) operator()(T1&& t1, T2&& t2) const
		{
			return t1 < t2 ? t2 : t1;
		}
	};

	struct min_t
	{
		typedef mfunction_tag tag;

		template<typename T1, typename T2>
		CCDK_FORCEINLINE  constexpr decltype(auto) operator()(T1&& t1, T2&& t2) const
		{
			return t1 < t2 ? t1 : t2;
		}
	};

	struct condi_t
	{
		typedef mfunction_tag tag;

		template<typename T1, typename T2>
		CCDK_FORCEINLINE constexpr  decltype(auto) operator()(bool condi, T1&& t1, T2&& t2)  const
		{
			return condi ? t1 : t2;
		}
	};

	struct deref_member
	{
		template<typename Class, typename Member>
		CCDK_FORCEINLINE constexpr decltype(auto) operator()(const Class& clazz, const Member& member )
		{
			return clazz->*member;
		}
	};
			
	constexpr post_dec_t post_dec{};
	constexpr post_inc_t post_inc{};
	constexpr index_t    index{};
	constexpr arrow_t    arrow{};
	constexpr invoke_t   invoke{};
	constexpr condi_t    condi{};
	constexpr max_t      max{};
	constexpr min_t      min{};

#define CCDK_DEFINE_BINARY_FN_OBJECT(name, op,...) struct name ## _t { typedef mfunction_tag tag; template<typename T1, typename T2> CCDK_FORCEINLINE constexpr decltype(auto) operator()(T1&& t1, T2&& t2) const noexcept { return t1 op t2; } };  constexpr name ## _t name{};
#define CCDK_DEFINE_UNIARY_FRONT_FN_OBJECT(name, op,...) struct name ## _t { typedef mfunction_tag tag; template<typename T> constexpr CCDK_FORCEINLINE decltype(auto) operator()(T&& t) const noexcept { return op t; } }; constexpr name ## _t name{};

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
			
ccdk_namespace_mpl_fn_end