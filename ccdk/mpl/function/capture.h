#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/base/enable_if.h>
#include<ccdk/mpl/base/or_.h>
#include<ccdk/mpl/type_traits/decay.h>
#include<ccdk/mpl/type_traits/is_function.h>
#include<ccdk/mpl/type_traits/is_function_obj.h>
#include<ccdk/mpl/fusion/tuple_storage.h>
#include<ccdk/mpl/function/partial.h>
#include<ccdk/mpl/function/create.h>

ccdk_namespace_mpl_fn_start

	/* capture( args...)(fn) => fn( args...)  */
	template<typename... Args>
	struct capture_t
	{
		static constexpr uint32 L = sizeof...(Args);
		typedef fs::tuple_storage<L, make_indice<L>, decay_t<Args>...> value_type;
		typedef capture_t type;
		value_type storage;

		CCDK_FORCEINLINE constexpr capture_t(Args... args) :storage{ util::forward<Args>(args)... } {}

		/* bind part Args... with Fn */
		template< typename Fn, uint32... indice >
		CCDK_FORCEINLINE constexpr auto _invoke_impl( Fn&& fn, indice_pack<indice...> ) const noexcept
		{
			return partial( util::forward<Fn>(fn), fs::ebo_at<indice>(util::move(storage))... );
		}

		/* function ptr */
		template< typename Fn, typename = check_t< is_function<Fn> > >
		CCDK_FORCEINLINE constexpr auto operator()(Fn* fn ) const noexcept
		{
			return _invoke_impl( util::forward<Fn*>(fn), make_indice<L>{} );
		}

		/* function object or normal function */
		template< typename Fn, typename = check_t< or_< is_function<Fn>, is_function_obj<Fn>>>   >
		CCDK_FORCEINLINE constexpr auto operator()( Fn& fn ) const noexcept
		{
			return _invoke_impl( util::forward<Fn&>(fn), make_indice<L>{} );
		}
	};
		
	constexpr create_t< capture_t > capture{};

ccdk_namespace_mpl_fn_end