#pragma once

#include<ccdk/type.h>
#include<ccdk/mpl/type_traits/forward.h>
#include<ccdk/mpl/type_traits/move.h>
#include<ccdk/mpl/container/tuple_storage.h>
#include<ccdk/mpl/function/create.h>

namespace ccdk
{
	namespace mpl
	{
		// fmap(f)( gs...)(args...)
		// = f( gs(args...)...)
		namespace fn_detail
		{
			template<typename Fn, typename... Fs>
			struct fmap_t
			{
				static constexpr uint32 L = sizeof...(Fs);
				typedef fmap_t<Fn, Fs...> type;
				typedef tuple_storage<L - 1, make_indice<L - 1>, Fs...> value_type;
				Fn fn;
				value_type storage;

				CCDK_FORCEINLINE constexpr
				fmap_t(Fn&& inFn, Fs&&... fs)
					: fn{ util::move(inFn) }, storage{ util::move(fs)... }
				{}

				CCDK_FORCEINLINE constexpr 
				fmap_t(Fn const& inFn, Fs const&... fs)
					: fn(inFn), storage{ fs... }
				{}


				template<
					typename T,
					typename... Args,
					uint32... indice
				>
				CCDK_FORCEINLINE constexpr
				auto __invoke_impl(
					indice_pack<indice...>,
					T&& t,
					Args&&... args) const
				{
					return fn(ebo_at<indice>(util::move(storage))(args...)...);
				}


				template<
					typename T,
					typename... Args
				>
				CCDK_FORCEINLINE constexpr 
				auto operator()(T&& t, Args&&... args) const
				{
					static_assert(sizeof...(Args)+1 == L,
						"functions and parameters not match");
					return __invoke_impl(
						make_indice<L>,
						util::forward<T>(t),
						util::forward<Args>(args)...
					);
				}
			};
		}

		constexpr fn_detail::create< fn_detail::fmap_t >  fmap{};
	}
}