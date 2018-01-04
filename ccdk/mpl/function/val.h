#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/type_traits/decay.h>
#include<ccdk/mpl/util/create.h>
#include<ccdk/mpl/function/function_fwd.h>

ccdk_namespace_mpl_fn_start
			
	template<typename T>
	class expr< value_t<T> >
	{
	public:
		typedef expr type;
		typedef decay_t<T> value_type;
		typedef mfunction_tag     tag;

		static constexpr uint32 size = 0;	     /* args length */
		static constexpr uint32 wild_size = 0;   /* wild placeholder count of sub-expr(e.g. _ ) */
		static constexpr uint32 index_size = 0;  /* max index-placeholder count of sub-expr(e.g. 2_ ) */

		value_type t;

		/*value constructor*/

		constexpr explicit expr() :t{} {}

		template<typename T2>
		CCDK_FORCEINLINE constexpr explicit expr(T2&& inT)  :t{ util::forward<T2>(inT) } {}

		/*to value type*/
		CCDK_FORCEINLINE constexpr explicit operator const value_type&() const noexcept { return t; DebugFunctionName("=========="); }

		/*eval lazy*/
		template<uint32 Index, typename... Args> 
		CCDK_FORCEINLINE const value_type& eval(Args const& ...) const noexcept { return t; }

		/*eval lazy*/
		template<typename... Args>
		CCDK_FORCEINLINE constexpr  const value_type&  operator()(Args const& ...) const noexcept { return t; }
	};

	template<typename T> using val_t = expr< value_t<T> >;
	constexpr util::create_t< val_t > val{};

ccdk_namespace_mpl_fn_end