#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/util/forward.h>
#include<ccdk/mpl/util/move.h>
#include<ccdk/mpl/util/create.h>
#include<ccdk/mpl/fusion/imap.h>

ccdk_namespace_mpl_fn_start

	/* fmap(f)( gs...)(args...)  => f( gs(args...)...) */
	template<typename Fn, typename... Fs>
	struct fmap_t
	{
		static constexpr uint32 size = sizeof...(Fs);
		typedef fmap_t this_type;

		Fn fn;
		fs::closure_args< size, Fs...> fs;

		CCDK_FORCEINLINE constexpr fmap_t(Fn&& inFn, Fs&&... inFs) : fn{ util::forward<Fn>(inFn) }, fs{ util::forward<Fs>(inFs)... } {}


		template< typename... Args, uint32... indice >
		CCDK_FORCEINLINE constexpr auto _invoke_impl( indice_pack<indice...>, Args&&... args) const
		{
			return fn(fs.template at<indice>()( util::forward<Args>(args)...)...);
		}


		template< typename T, typename... Args >
		CCDK_FORCEINLINE constexpr  auto operator()(T&& t, Args&&... args) const
		{
			static_assert(sizeof...(Args)+1 == size, "functions and parameters not match");
			return _invoke_impl( make_indice<size>{}, util::forward<T>(t), util::forward<Args>(args)... );
		}
	};

	constexpr util::create_t< fmap_t >  fmap{};

ccdk_namespace_mpl_fn_end