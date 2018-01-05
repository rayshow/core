#pragma once

#include<ccdk/compile.h>
#include<ccdk/mpl/mcontainer/mcontainer_fwd.h>

namespace ccdk
{
	namespace mpl
	{
		template<>
		struct arg_pack<>
		{
			typedef arg_pack          type;
			typedef arg_pack          begin;
			typedef arg_pack          end;
			typedef forward_category  category;
			static constexpr uint32 size = 0;
			static constexpr bool   empty = true;

			/* can't deref */
			/* can't pop_front */
			/* no front */
			/* no next */

			template<typename T> struct merge;

			template<typename... Args2> struct merge< arg_pack<Args2...>>
			{
				typedef arg_pack<Args2...> type;
			};
		};
		
		template<typename T, typename... Args> 
		struct arg_pack<T, Args...>
		{
			typedef forward_category  category;
			typedef arg_pack          type;
			typedef arg_pack          begin;
			typedef arg_pack<>        end;
			typedef arg_pack<Args...> next;
			typedef T                 deref;
			typedef arg_pack<Args...> pop_front;
			typedef T                 front;
			static constexpr uint32 size  = sizeof...(Args)+1;
			static constexpr bool   empty = false;

			template<typename P>
			struct push_front
			{
				typedef arg_pack<P, T, Args...> type;
			};

			template<typename T> struct merge;

			template<typename... Args2> struct merge< arg_pack<Args2...>>
			{
				typedef arg_pack<T,Args..., Args2...> type;
			};
		};
	}
}