#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/base/enable_if.h>
#include<ccdk/mpl/base/logic_.h>
#include<ccdk/mpl/type_traits/decay.h>
#include<ccdk/mpl/type_traits/is_function.h>
#include<ccdk/mpl/type_traits/is_function_obj.h>
#include<ccdk/mpl/fusion/imap.h>
#include<ccdk/mpl/function/partial.h>
#include<ccdk/mpl/util/create.h>

ccdk_namespace_mpl_fn_start

	/* capture( args...)(fn) => fn( args...)  */
	template<typename... Args>
	struct capture_t
	{
		static constexpr uint32 size = sizeof...(Args);
		typedef capture_t this_type;

		fs::closure_args< size,  Args...> content;

		CCDK_FORCEINLINE constexpr capture_t(Args&&... args) :content{ util::forward<Args>(args)... } {  }

		/* bind part Args... with Fn */
		template< typename Fn, uint32... indice >
		CCDK_FORCEINLINE constexpr auto _invoke_impl(  Fn&& fn, indice_pack<indice...> ) noexcept
		{
			return partial( util::forward<Fn>(fn), util::move(content.template at<indice>() )... );
		}
		
		/* function / function pointer */
		template< typename Fn>
		CCDK_FORCEINLINE constexpr auto operator()(Fn&& fn ) noexcept
		{
			DebugFunctionName();
			return _invoke_impl( util::forward<Fn>(fn), make_indice<size>{} );
		}
	};
		
	constexpr util::create_t< capture_t > capture{};

ccdk_namespace_mpl_fn_end