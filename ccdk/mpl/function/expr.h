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
		namespace fn_impl
		{
			//mark expr
			struct lazy_t {};
			template<> struct is_lazy_t<lazy_t> :true_ {};
			template<typename T> struct is_lazy_t :false_ {};
			constexpr lazy_t lazy{};

			// acculate args::W...
			template<typename... Args>
			struct acc_wph_count :uint_<0>
			{};

			template<typename T, typename... Args>
			struct acc_wph_count<T, Args...>
				:uint_< T::W +acc_wph_count<Args...>::value > {};

			//max args::I...
			template<typename... Args>
			struct max_iph_count :uint_<0> {};

			template<typename T, typename... Args>
			struct max_iph_count<T,Args...>
				:uint_< op::max(T::I, max_iph_count<Args...>::value) > {};

			template<uint32 acc, typename pack, typename... Args>
			struct pack_wph_step
			{
				typedef pack type;
			};

			//make shift indice
			template<uint32 acc, uint32... indice, typename T, typename... Args>
			struct pack_wph_step< acc, indice_pack<indice...>, T, Args...>
				: pack_wph_step< acc + T::W, indice_pack<indice..., acc>, Args...>{};

			template<typename... Args>
			struct expr;

			template<typename T> struct is_expr :false_ {};
			template<typename... Args> struct is_expr< expr<Args...> > :true_ {};

			//operation expr
			template<typename Fn, typename... Args>
			struct expr<Fn, Args...>
			{
				static constexpr uint32 L = sizeof...(Args);				 //args length
				static constexpr uint32 W = acc_wph_count< Args...>::value;  //wild placeholder count of sub-expr(e.g. _ )
				static constexpr uint32 I = max_iph_count< Args...>::value;  //max index-placeholder count of sub-expr(e.g. 2_ )
				typedef expr           type;
				typedef make_indice<L> indice;
				typedef tuple_storage<L, indice, decay_t<Args>...> value_type;
				static constexpr typename pack_wph_step<0, indice_pack<>, Args...>::type shifts_indice{};
				static constexpr typename indice args_indice{};
			
				decay_t<Fn>  fn;
				value_type storage;

				CCDK_FORCEINLINE constexpr
					expr(Args... args)
					:storage{ util::forward<Args>(args)... }
				{}

				template<
					uint32 Start,
					typename Ctx,
					uint32... idx,
					uint32... sft
				>
				CCDK_FORCEINLINE constexpr
				decltype(auto)
				__eval_impl(const Ctx& ctx, indice_pack<idx...>, indice_pack<sft...>) const
				{
					return fn( ebo_at< idx >(storage).template eval<Start + sft>(ctx)...);
				}

				template<
					uint32 Start,
					typename Ctx
				>
				CCDK_FORCEINLINE constexpr
				decltype(auto)
				eval(const Ctx& ctx)
				{
					return __eval_impl<Start>(ctx, args_indice, shifts_indice);
				}

				//create lazy invoke expr
				template<typename... Args1>
				CCDK_FORCEINLINE constexpr
				decltype(auto)
				__invoke_impl_is_lazy(true_, Args1&&... args1)
				{
					return expr< op::invoke_t, Args1...>{ args1... };
				}

				//not lazy invoke , then eval expr
				template<typename... Args1>
				CCDK_FORCEINLINE constexpr
				decltype(auto)
				__invoke_impl_is_lazy(false_, Args1&&... args1)
				{
					return eval<0>(ct::create_ref_tuple(util::forward<Args1>(args1)...));
				}

				// no parameter, just eval expr
				CCDK_FORCEINLINE constexpr
				decltype(auto)
				__invoke_impl_len(uint_<0>)
				{
					static_assert(W == 0 && I == 0, "has placeholder but execute with no parameter ");
					return __invoke_impl_is_lazy(false_{});
				}

				// has parameter, judge is lazy 
				template<
					uint32 len,
					typename T,
					typename... Args1
				>
				CCDK_FORCEINLINE constexpr
				decltype(auto)
				__invoke_impl_len(uint_<len>,T&& t, Args1&&... args1)
				{
					return __invoke_impl_is_lazy(typename is_lazy_t<T>::type{},
						util::forward<Args1>(args1)...);
				}

				//enter point
				template<typename... Args1>
				CCDK_FORCEINLINE constexpr 
				decltype(auto)
				operator()(Args1&&... args1)
				{
					return __invoke_impl_len(uint_c<sizeof...(Args1)>,
							util::forward<Args1>(args1)...);
				}
			};

			//wild match placeholder _
			template<>
			struct expr< null_ >
			{

			};

			//index match placeholder 1_ / 2_ ...
			template<uint32 index>
			struct expr< uint_<index> >
			{

			};

			template<
				typename T,
				typename = check_t< not_< is_expr<T>>>,  
				typename... Args
			>
			CCDK_FORCEINLINE constexpr
			auto 
			operator+(const expr<Args...>& e, T&& t)
			{
				return expr<op::add_t, expr<Args...>, T>{
					const_cast< expr<Args...>&&>(e),
					util::forward<T>(t)
				};
			}

			template<
				typename T,
				typename = check_t< not_< is_expr<T>>>,
				typename... Args
			>
				CCDK_FORCEINLINE constexpr
				auto
				operator+(T&& t, const expr<Args...>& e)
			{
				return expr<op::add_t, expr<Args...>, T>{
						util::forward<T>(t),
						const_cast< expr<Args...>&&>(e)
				};
			}

			template<
				typename... Args1,
				typename... Args2,
			>
				CCDK_FORCEINLINE constexpr
				auto
				operator+(const expr<Args1...>& e1, const expr<Args2...>& e2)
			{
				return expr<op::add_t, expr<Args1...>, expr<Args2...> >{
						const_cast< expr<Args2...>&&>(e1),
						const_cast< expr<Args2...>&&>(e2)
				};
			}
		}

		namespace fn
		{
			constexpr fn_impl::expr<null_> _{};
		}
	}
}