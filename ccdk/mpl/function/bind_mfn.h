#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/base/enable_if.h>
#include<ccdk/mpl/type_traits/remove_const.h>
#include<ccdk/mpl/type_traits/is_mfn_ptr.h>
#include<ccdk/mpl/type_traits/result_of.h>
#include<ccdk/mpl/type_traits/args_of.h>
#include<ccdk/mpl/util/addressof.h>

ccdk_namespace_mpl_fn_start
	
	/* bind_mfn( class::*fn, class/class* )( args...) => class*->*fn( args... ) */
	template<
		typename Fn, typename Class, typename Ret,
		typename... Args
	>
	struct member_function_t
	{
		Fn  fn;      /* copy fn object */
		Class *clz;  /* track class pointer */

		/* default */
		CCDK_FORCEINLINE constexpr member_function_t(const member_function_t&) = default;

		/* value constructor */
		CCDK_FORCEINLINE constexpr member_function_t(Fn inFn, Class* inClz) noexcept :fn(inFn), clz(inClz) {}

		/* invoke */
		CCDK_FORCEINLINE constexpr Ret operator()(Args... args) const noexcept
		{
			return (clz->*fn)(args...);
		}
	};

	struct bind_mfn_t
	{
		template<
			typename Fn, typename Class, typename Ret,
			typename... Args
		>
		CCDK_FORCEINLINE constexpr auto _bind_mfn_impl(Fn fn, Class* clz, arg_pack<Args...>) const noexcept
		{
			DebugFunctionName();
			return member_function_t< Fn, Class, Ret, Args...>(fn, clz);
		}

		template<
			typename Fn,
			typename FnArgs = args_of<Fn>,
			typename Class = typename FnArgs::clazz,
			typename = check_t< is_mfn_ptr<Fn>>
		>
		CCDK_FORCEINLINE constexpr auto operator()(Fn fn, Class& clz)const noexcept
		{
			typedef remove_const_t<Class> NonConstClass;
			return _bind_mfn_impl<Fn, NonConstClass, result_of_t<Fn> >(
				fn,
				(NonConstClass*)(util::addressof(clz)),
				typename FnArgs::type{}
			);
		}

		template<
			typename Fn,
			typename FnArgs = args_of<Fn>,
			typename Class = typename FnArgs::clazz,
			typename = check_t< is_mfn_ptr<Fn>>
		>
		CCDK_FORCEINLINE constexpr auto operator()(Fn fn, Class* clz) const noexcept
		{
			typedef remove_const_t<Class> NoConstClass;
			return _bind_mfn_impl<Fn, NoConstClass, result_of_t<Fn> >(
				fn, (NoConstClass*)clz,
				typename FnArgs::type{}
			);
		}
	};

	constexpr bind_mfn_t bind_mfn{};

ccdk_namespace_mpl_fn_end