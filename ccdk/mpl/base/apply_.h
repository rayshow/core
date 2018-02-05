#pragma once

#include<ccdk/mpl/mpl_module.h>

ccdk_namespace_mpl_start

#define CCDK_MPL_TFN(T)  template<typename...> typename T
#define CCDK_MPL_TFN1(T) template<typename, typename...> typename T

/* meta-function apply */
template<typename MFn, typename... Args>
struct apply_:MFn::template apply<Args...> {};

template<typename MFn, typename... Args>
using apply_t = typename apply_<MFn,Args...>::type;

/* template-function apply */
template<CCDK_MPL_TFN(TFn), typename... Args> struct call_:TFn<Args...> {};

/* partial add apply */
template<CCDK_MPL_TFN(TFn), typename... Args>
struct add_apply_
{
	template<typename... Args2> struct apply:TFn<Args..., Args2...> {};
};

/* reverse partial apply */
template<CCDK_MPL_TFN(TFn), typename... Args>
struct reverse_add_apply_
{
	template<typename... Args2> struct apply :TFn<Args2..., Args...> {};
};

/* replace call */
namespace mpl_impl
{
	template<typename T> struct replace_call_impl;

	template<CCDK_MPL_TFN(TFN), typename... Args>
	struct replace_call_impl< TFN<Args...> >
	{
		template<typename... Args2>
		struct apply :TFN<Args...> 
		{
			static_assert(sizeof...(Args) == sizeof...(Args2),
				"replace call Args1 size need be same with Args2");
		};
	};

	template<typename Fn, typename... Args>
	struct replace_call_ : mpl_impl::replace_call_impl<Fn>::template apply<Args...>{};
}


ccdk_namespace_mpl_end