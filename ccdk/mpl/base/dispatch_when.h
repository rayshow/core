#include<ccdk/mpl/base/and_.h>
#include<ccdk/mpl/base/or_.h>
#include<ccdk/mpl/base/enable_if.h>
#include<ccdk/mpl/type_traits/has_inner_type.h>
#include<ccdk/mpl/type_traits/is_function_obj.h>
#include<ccdk/mpl/container/arg_pack.h>
#include<ccdk/mpl/function/arg.h>

namespace ccdk
{
	namespace mpl
	{
		//B is a static justify template class
		template<typename B, typename = check<has_inner_value_v<B,bool>> >
		struct static_case:public B
		{
			//void* ptr;
			//Fn need to be a lambda or function obj
			template<typename Fn, typename = check< is_function_obj_v<Fn> >>
			static_case(Fn fn)
			{

			}
		};

		struct is_match_case
		{
			template<typename T> struct apply:public T{};
		};
		
		struct static_dispatch_t
		{
			template<typename D, typename... Cs>
			constexpr auto operator()(D d, Cs... cs)
			{
				constexpr uint32 index = arg_pack_find_if_v<is_match_case, D, Cs...>;
				auto Fn = arg<index>(cs..., d);
				return Fn();
			}
		};

		template<typename D, typename... Cs>
		inline constexpr decltype(auto) dispatch(D d, Cs... cs)
		{
			constexpr uint32 index = arg_pack_find_if_v<is_match_case, D, Cs...>;
			auto Fn = arg<index>(cs..., d);
			return Fn();
		}

	}
}