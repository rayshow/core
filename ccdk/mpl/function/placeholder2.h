#pragma once

#include<ccdk/type.h>
#include<ccdk/mpl/base/null_.h>
#include<ccdk/mpl/base/int_.h>
#include<ccdk/mpl/base/enable_if.h>
#include<ccdk/mpl/base/val_pack.h>
#include<ccdk/mpl/type_traits/decay.h>
#include<ccdk/mpl/util/move.h>
#include<ccdk/mpl/container/ref_tuple.h>
#include<ccdk/mpl/container/tuple_storage.h>
#include<ccdk/mpl/function/operator.h>


namespace ccdk
{
	namespace mpl
	{
		namespace ph
		{

			//mark expr
			struct lazy_t {}; 
			constexpr lazy_t lazy{};

			template<typename T>
			struct is_lazy_t :false_ {};

			template<typename T>
			struct is_lazy_t<lazy_t>:true_{};

			template<uint32 N, typename Fn, typename... Args>
			struct expr
			{
				static_assert(N == sizeof...(Args), "Args size need equal to N");
				typedef expr           type;
				typedef make_indice<N> indice;
				typedef tuple_storage<N, indice,  decay_t<Args>...> value_type;

				decay_t<Fn>  fn;
				value_type storage;

				CCDK_FORCEINLINE constexpr
					expr(Fn inFn, Args... args)
					: fn(util::forward<Fn>(inFn)),
					storage{ util::forward<Args>(args)... }
				{}


				template<uint32 Start, typename Ctx>
				CCDK_FORCEINLINE constexpr
					decltype(auto)
					eval(Ctx& ctx) const
				{

				}


				//create expr
				template<typename... Args1>
				CCDK_FORCEINLINE constexpr
					decltype(auto)
					__invoke_impl(true_, Args1&&... args1)
				{

				}

				//eval
				template<typename... Args1>
				CCDK_FORCEINLINE constexpr
					decltype(auto)
					__invoke_impl(false_, Args1&&... args1)
				{

				}


				//enter point
				template<typename T, typename... Args1>
				CCDK_FORCEINLINE constexpr 
					decltype(auto)
					operator()(T&& t, Args1&&... args1)
				{
					
				}

			};




			template<typename Fn>
			using teminal = expr<0, Fn>;

		}
	}
}