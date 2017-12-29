#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/util/move.h>
#include<ccdk/mpl/util/forward.h>
#include<ccdk/mpl/fusion/tuple_storage.h>
#include<ccdk/mpl/function/create.h>

ccdk_namespace_mpl_fn_start

	/*  combine(f , g, h, ...)(args...)  => ...h( g( f(args...))) */
	template<typename Fn, typename... Gs>
	struct combine_t
	{
		static constexpr uint32 L = sizeof...(Gs)+1;
		typedef combine_t<Fn, Gs...> type;
		typedef fs::tuple_storage<L, make_indice<L>, Fn, Gs...> value_type;
		value_type storage;

		CCDK_FORCEINLINE constexpr 
		combine_t(Fn const& fn, Gs const& ... gs) : storage{ fn, gs... } {}

		CCDK_FORCEINLINE constexpr 
		combine_t(Fn && fn, Gs && ... gs) : storage{  util::move(fn),  util::move(gs)...  } {}

		template<typename... Args> 
		CCDK_FORCEINLINE constexpr decltype(auto) _invoke_impl(uint_<0>, Args&&... args)
		{
			return fs::ebo_at<0>(util::move(storage))(args...);
		}

		template< uint32 index, typename... Args >
		CCDK_FORCEINLINE constexpr decltype(auto) _invoke_impl( uint_<index>, Args&&... args )
		{
			return fs::ebo_at<index>(util::move(storage))(
					_invoke_impl( uint_<index - 1>{}, util::forward<Args>(args)... )
			);
		}

		template<typename... Args>
		CCDK_FORCEINLINE constexpr decltype(auto) operator()(Args&&... args)
		{
			return _invoke_impl( uint_<L-1>{}, util::forward<Args>(args)... );
		}
	};
		
	constexpr create_t<combine_t> combine{};

ccdk_namespace_mpl_fn_end