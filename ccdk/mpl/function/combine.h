#pragma once

#include<ccdk/type.h>
#include<ccdk/mpl/util/move.h>
#include<ccdk/mpl/util/forward.h>
#include<ccdk/mpl/container/tuple_storage.h>
#include<ccdk/mpl/function/create.h>


namespace ccdk
{
	namespace mpl
	{
		namespace fn_detail
		{
			template<typename Fn, typename... Gs>
			struct combine_t
			{
				static constexpr uint32 L = sizeof...(Gs)+1;
				typedef combine_t<Fn, Gs...> type;
				typedef tuple_storage<L, make_indice<L>, Fn, Gs...> value_type;
				value_type storage;

				constexpr combine_t(Fn const& fn, Gs const& ... gs)
					: storage{ fn, gs... }
				{}

				constexpr combine_t(Fn && fn, Gs && ... gs)
					: storage{ 
						util::move(fn), 
						util::move(gs)... 
					}
				{}

				template<typename... Args>
				CCDK_FORCEINLINE
				auto __invoke_impl(uint_<0>, Args&&... args)
				{
					return ebo_at<0>(util::move(storage))(args...);
				}

				template<
					uint32 index,
					typename... Args
				>
				CCDK_FORCEINLINE
				auto __invoke_impl(
					uint_<index>,
					Args&&... args
				)
				{
					return ebo_at<index>(util::move(storage))(
						__invoke_impl(
							uint_<index - 1>{},
							util::forward<Args>(args)...
						)
					);
				}

				template<typename... Args>
				CCDK_FORCEINLINE constexpr
				auto operator()(Args&&... args)
				{
					return __invoke_impl(
						uint_<L-1>{},
						util::forward<Args>(args)...
					);
				}
			};
		}

		namespace fn
		{
			constexpr fn_detail::create<fn_detail::combine_t> combine{};
		}
		
	}
}