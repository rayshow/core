#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/util/move.h>
#include<ccdk/mpl/util/forward.h>
#include<ccdk/mpl/util/create.h>
#include<ccdk/mpl/fusion/imap.h>
#include<ccdk/mpl/function/function_fwd.h>

ccdk_namespace_mpl_fn_start

	/*  combine(f , g, h, ...)(args...)  => ...h( g( f(args...))) */
	template<typename Fn, typename... Fs>
	struct combine_t
	{
		static constexpr uint32 size = sizeof...(Fs)+1;
		typedef combine_t<Fn, Fs...> this_type;
		typedef mfunction_tag tag;

		fs::closure_args< size, Fn, Fs...> content;

		CCDK_FORCEINLINE constexpr combine_t(Fn && fn, Fs && ... fs) : content{  util::forward<Fn>(fn),  util::forward<Fs>(fs)...  } {}

		template<typename... Args> 
		CCDK_FORCEINLINE constexpr decltype(auto) _invoke_impl(uint_<0>, Args&&... args)
		{
			return content.template at<0>()(util::forward<Args>(args)...);
		}

		template< uint32 index, typename... Args >
		CCDK_FORCEINLINE constexpr decltype(auto) _invoke_impl( uint_<index>, Args&&... args )
		{
			return content.template at<index>()( _invoke_impl( uint_<index - 1>{}, util::forward<Args>(args)... ) );
		}

		template<typename... Args>
		CCDK_FORCEINLINE constexpr decltype(auto) operator()(Args&&... args)
		{
			return _invoke_impl( uint_<size-1>{}, util::forward<Args>(args)... );
		}
	};
		
	constexpr util::create_t<combine_t> combine{};

ccdk_namespace_mpl_fn_end