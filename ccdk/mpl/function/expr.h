#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/preprocessor/seq_foreach_tuple.h>
#include<ccdk/mpl/base/null_.h>
#include<ccdk/mpl/base/or_.h>
#include<ccdk/mpl/base/enable_if.h>
#include<ccdk/mpl/base/val_pack.h>
#include<ccdk/mpl/base/arg_pack_first.h>
#include<ccdk/mpl/type_traits/decay.h>
#include<ccdk/mpl/util/move.h>
#include<ccdk/mpl/fusion/imap.h>
#include<ccdk/mpl/function/operator.h>
#include<ccdk/mpl/function/function_fwd.h>


ccdk_namespace_mpl_fn_start

	/* mark lazy expression */
	struct mark_lazy_t {};
	constexpr mark_lazy_t lazy_{};

	template<typename T> struct is_mark_lazy :false_ {};
	template<> struct is_mark_lazy<mark_lazy_t> :true_ {};

	/* acculate args::wild_size... */
	template<typename... Args>
	struct acc_wph_count :uint_<0> {};

	template<typename T, typename... Args>
	struct acc_wph_count<T, Args...> :uint_< T::wild_size + acc_wph_count<Args...>::value > {};

	/* max args::index_size... */
	constexpr uint32 u32_max(uint32 a, uint32 b) { return a < b ? b : a; }
	 
	/* acculate args::index_size... */
	template<typename... Args>
	struct max_iph_count :uint_<0> {};

	template<typename T, typename... Args>
	struct max_iph_count<T, Args...> :uint_< u32_max(T::index_size, max_iph_count<Args...>::value) > {};

	/* marke wild indice pack */
	template<uint32 acc, typename pack, typename... Args>
	struct pack_wph_step { typedef pack type; };

	template<uint32 acc, uint32... indice, typename T, typename... Args>
	struct pack_wph_step< acc, indice_pack<indice...>, T, Args...> : pack_wph_step< acc + T::wild_size, indice_pack<indice..., acc>, Args...> {};


	/* filter expr */
	template<typename T> struct filter_expr : if_ < is_expr<T>, T, expr < value_t<T>>> {};



	template<typename... Args>
	class expr_base
	{
	public:
		typedef expr<Args...> derive_type;

		constexpr expr_base() = default;

		template<typename T, typename = check_t< not_< is_expr<T>>> > 
		CCDK_FORCEINLINE constexpr auto operator[](T&& t) const
		{
			return expr< index_t, derive_type, expr< value_t<T> > >{ const_cast<expr<Args...>&&>(*this), expr< value_t<T>>{util::forward<T>(t)} };
		}

		template<typename... Args1>
		CCDK_FORCEINLINE constexpr auto operator[](expr<Args1...>&& e) const 
		{
			return expr< add_t, derive_type, expr<Args1...> >{ const_cast<derive_type&&>(*this), const_cast<expr<Args1...>&&>(e) };
		}

		



	};


	/* lazy and wild expr  */
	template<typename Fn, typename... Args>
	class expr<Fn, Args...> 
	{
	public:
		static constexpr uint32 size = sizeof...(Args);	                     /* args length */
		static constexpr uint32 wild_size =  acc_wph_count< Args...>::value; /* wild placeholder count of sub-expr(e.g. _ ) */
		static constexpr uint32 index_size = max_iph_count< Args...>::value; /* max index-placeholder count of sub-expr(e.g. 2_ ) */
		typedef expr                   this_type;
		typedef make_indice<size>      indice_type;
		typedef mfunction_tag          tag;
		static constexpr typename pack_wph_step<0, indice_pack<>, Args...>::type shifts_indice{};
		static constexpr indice_type args_indice{};

	private:
		/* closure of lazy eval */
		Fn                              fn;
		fs::closure_args<size, Args...> args;

		/* eval helper  */
		template< uint32 Start, typename Content, uint32... Index, uint32... Shift >
		CCDK_FORCEINLINE constexpr decltype(auto) _eval_impl(const Content& ctx, indice_pack<Index...>, indice_pack<Shift...>) const { return fn(args.template at<Index>().template eval<Start + Shift>(ctx)...); }

		/* explicit mark lazy,  create lazy invoke expr and return */
		template<typename... Args1>
		CCDK_FORCEINLINE constexpr decltype(auto) _invoke_impl_is_lazy(true_, Args1&&... args1) { return expr< invoke_t, Args1...>{ args1... }; }

		/* not lazy , eval expr */
		template<typename... Args1>
		CCDK_FORCEINLINE constexpr decltype(auto) _invoke_impl_is_lazy(false_, Args1&&... args1) { return eval<0>(fs::create_reference_args(util::forward<Args1>(args1)...)); }

		/* arg len > 0 , static dispatch  by weather args is mark lazy */
		template< uint32 len, typename T, typename... Args1 >
		CCDK_FORCEINLINE constexpr decltype(auto) _invoke_impl_len(uint_<len>, T&& t, Args1&&... args1) { return _invoke_impl_is_lazy(typename is_mark_lazy<T>::type{}, util::forward<T>(t), util::forward<Args1>(args1)...); }

		/* no parameter, just eval expr */
		CCDK_FORCEINLINE constexpr decltype(auto) _invoke_impl_len(uint_<0>) { static_assert(wild_size == 0 && index_size == 0, "has placeholder but execute with no parameter "); return _invoke_impl_is_lazy(false_{}); }

	public:
		ccdk_expr_lazy_assign


		/* value */
		template<typename... Args2, typename = check_t< and_< is_convertible<Args2, Args>...>>>
		CCDK_FORCEINLINE explicit constexpr  expr(Args2&&... args2) :args{ util::forward<Args2>(args2)... } {}

		/*template<typename... Args2>
		CCDK_FORCEINLINE explicit constexpr  expr(int, Args2&&... args2) : args{ util::forward<Args2>(args2)... } {  DebugFunctionName(); }
*/
		/* move */
		CCDK_FORCEINLINE constexpr expr(expr&& other) : fn{util::move(other.fn)}, args { util::move(other.args) } {}

		/*eval expr */
		template< uint32 Start, typename Content >
		CCDK_FORCEINLINE constexpr decltype(auto) eval(const Content& ctx) const { return _eval_impl<Start>(ctx, args_indice, shifts_indice); }

		/* eval enter point, check Args is explicit mark lazy or wild match placeholder  is valid with input args  */
		template< typename... Args1, typename = check_t <  or_<  is_mark_lazy< arg_pack_first_t<Args1...> >, bool_< sizeof...(Args1) == u32_max(wild_size, index_size)> >> >
		CCDK_FORCEINLINE constexpr decltype(auto) operator()(Args1&&... args1) { return _invoke_impl_len(uint_c<sizeof...(Args1)>, util::forward<Args1>(args1)...); }
	};

	/* wild match placeholder _ */
	template<>
	class expr< null_ >
	{
	public:
		static constexpr uint32 size = 0;	    /* args length */
		static constexpr uint32 wild_size = 1;  /* wild placeholder count of sub-expr(e.g. _ ) */
		static constexpr uint32 index_size = 0; /* max index-placeholder count of sub-expr(e.g. 2_ ) */
		typedef expr            this_type;

		ccdk_expr_lazy_assign;

		template<typename T>
		CCDK_FORCEINLINE constexpr auto operator()(mark_lazy_t, T&& t) const
		{
			return expr< invoke_t, typename filter_expr<T>::type>{ typename filter_expr<T>::type{ util::forward<T>(t) } };
			//return expr< invoke_t, /*this_type ,*/typename filter_expr<T>::type>{ 1,/* *this,*/ util::forward<T>(t) };
		}

		constexpr expr() = default;
		constexpr expr(const expr& e) = default;

		/*  eval expr */
		template<uint32 Start, typename Context>
		CCDK_FORCEINLINE constexpr decltype(auto) eval(const Context& ctx) const noexcept { return ctx.template at<Start>(); }
	};

	/* index match placeholder 1_ / 2_ ... */
	template<uint32 index>
	struct expr< uint_<index> > 
	{
		static constexpr uint32 size = 0;			/* args length */
		static constexpr uint32 wild_size = 0;      /* wild placeholder count of sub-expr(e.g. _ ) */
		static constexpr uint32 index_size = index; /* max index-placeholder count of sub-expr(e.g. 2_ ) */
		typedef expr            this_type;
		ccdk_expr_lazy_assign


		constexpr expr() = default;
		constexpr expr(const expr& e) = default;

		/* eval expr  */
		template<uint32 Start, typename Context> 
		CCDK_FORCEINLINE decltype(auto) eval(const Context& ctx) const noexcept { return ctx.template at<index-1>(); }
	};

	/* placeholder */
	namespace ph
	{
		/* _ */
		constexpr expr<null_> _{};

		/* 1_ , 2_ ... */
		template<char... args>
		constexpr auto operator""_()
		{
			static_assert(val_first<char, args...>::value != '0', "number placeholder can't be 0_, need greater then 0");
			return expr < uint_ < literals::parse_integer<sizeof...(args)>({ args... }) > > {};
		}
	}


	/* expr + any T(exclude expr) */
	template< typename T, typename = check_t< not_< is_expr<T>>>, typename... Args >
	CCDK_FORCEINLINE constexpr auto operator+(const expr<Args...>& e, T&& t)
	{
		return expr< add_t, expr<Args...>, expr< value_t<T> > >{ const_cast<expr<Args...>&&>(e), expr< value_t<T>>{util::forward<T>(t)} };
	}

	/* any T(exclude expr) + expr */
	template< typename T, typename = check_t< not_< is_expr<T>>>, typename... Args >
	CCDK_FORCEINLINE constexpr auto operator+(T&& t, const expr<Args...>& e)
	{
		return expr< add_t, expr< value_t<T>>, expr<Args...> >{ util::forward<T>(t), const_cast<expr<Args...>&&>(e) };
	}

	/* expr + expr */
	template< typename... Args1, typename... Args2 >
	CCDK_FORCEINLINE constexpr auto operator+(const expr<Args1...>& e1, const expr<Args2...>& e2)
	{
		return expr< add_t, expr<Args1...>, expr<Args2...> >{ const_cast<expr<Args1...>&&>(e1), const_cast<expr<Args2...>&&>(e2) };
	}


#define BINARY_OPERATION_DECL(name, sign,...) \
	template< typename T, typename = check_t< not_< is_expr<T>>>, typename... Args >                                                         \
	CCDK_FORCEINLINE constexpr auto operator sign(const expr<Args...>& e, T&& t)                                                             \
	{ return expr< name ## _t, expr<Args...>, expr< value_t<T>> >{ const_cast<expr<Args...>&&>(e), expr< value_t<T>>{util::forward<T>(t)}};} \
                                                                                                                                             \
	template< typename T, typename = check_t< not_< is_expr<T>>>, typename... Args >                                                         \
	CCDK_FORCEINLINE constexpr auto operator sign(T&& t, const expr<Args...>& e)                                                             \
	{ return expr< name ## _t, expr< value_t<T>>, expr<Args...> >{ util::forward<T>(t), const_cast<expr<Args...>&&>(e) }; }                  \
                                                                                                                                             \
	template< typename... Args1, typename... Args2 >                                                                                         \
	CCDK_FORCEINLINE constexpr auto operator sign(const expr<Args1...>& e1, const expr<Args2...>& e2)                                        \
	{ return expr< name ## _t, expr<Args1...>, expr<Args2...> >{ const_cast<expr<Args1...>&&>(e1), const_cast<expr<Args2...>&&>(e2) }; } 

	/* same as operator+ */
	// require operator= 
	CCDK_PP_FOREACH_TUPLE(BINARY_OPERATION_DECL, (
		(sub, -),
		(div, / ),
		(mul, *),
		(mod, %),
		(shl, << ),
		(shr, >> ),
		(logic_and, &&),
		(logic_or, || ),
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
		(lequal, <= ),
		(gequal, >= ),
		(equal, == ),
		(nequal, != )));

ccdk_namespace_mpl_fn_end