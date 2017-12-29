#pragma once

#include<ccdk/mpl/mpl_module.h>


ccdk_namespace_mpl_fn_start


template<typename Fn, typename... Fs>
struct overload :public overload<Fn>, public overload<Fs...>
{
	using overload<Fn>::operator();
	using overload<Fs...>::operator();

	CCDK_FORCEINLINE constexpr overload(Fn* fn, Fs*... fs) :overload<Fn>(fn), overload<Fs...>(fs...) {}
};


template<typename Ret, typename... Args>
struct overload<Ret(Args...)>
{
	typedef Ret(*value_type)(Args...);
	
	value_type ptr;

	CCDK_FORCEINLINE constexpr overload(value_type inPtr) :ptr{ inPtr } {}

	CCDK_FORCEINLINE constexpr decltype(auto)  operator()(Args... args) { return ptr(args...); }
};


ccdk_namespace_mpl_fn_end