#pragma once

#include<ccdk/type.h>
#include<ccdk/mpl/util/forward.h>
#include<ccdk/mpl/util/move.h>
#include<ccdk/mpl/container/tuple_storage.h>
#include<ccdk/mpl/function/create.h>

namespace ccdk
{
	namespace mpl
	{
		// dispatch(f , gs...)(args...)
		// = f( gs(args)...)
		namespace fn_impl
		{
			template<typename Fn, typename... Fs>
			struct dispatch_t
			{
				static constexpr uint32 L = sizeof...(Fs);
				typedef dispatch_t type;
				typedef tuple_storage<L, make_indice<L>, Fs...> value_type;

				Fn fn;
				value_type storage;
				
				CCDK_FORCEINLINE constexpr 
				dispatch_t( Fn&& inFn, Fs&&... fs)
					: fn{ util::move(inFn) },
					storage{ util::move(fs)... }
				{}

				CCDK_FORCEINLINE constexpr
				dispatch_t( Fn const& inFn, Fs const&... fs)
					: fn(inFn),
					storage{ fs... }
				{}


				template<
					typename... Args,
					uint32... indice
				>
				CCDK_FORCEINLINE constexpr
				decltype(auto)
				__invoke_impl(indice_pack<indice...>, Args&&... args) const
				{
					return fn(ebo_at<indice>(util::move(storage))(args)...);
				}

				template<
					typename T,
					typename... Args
				>
				CCDK_FORCEINLINE constexpr
				decltype(auto)
				operator()( T&& t, Args&&... args) const
				{
					static_assert(sizeof...(Args)+1 == L,
						"functions and parameters not match");
					return __invoke_impl(
						make_indice<L>{},
						util::forward<T>(t),
						util::forward<Args>(args)...
					);
				}
			};


		}

		namespace fn
		{
			constexpr fn_impl::create< fn_impl::dispatch_t >  dispatch{};
		}
		
	}
}