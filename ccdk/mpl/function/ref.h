#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/type_traits/is_rref.h>
#include<ccdk/mpl/type_traits/remove_const.h>
#include<ccdk/mpl/util/move.h>
#include<ccdk/mpl/function/create.h>
#include<ccdk/mpl/function/expr.h>

ccdk_namespace_mpl_fn_start

	template<typename T>
	struct expr< reference_t<T> >
	{
		typedef expr type;
		typedef T& value_type;
		static constexpr uint32 L = 0;	//args length
		static constexpr uint32 W = 0;  //wild placeholder count of sub-expr(e.g. _ )
		static constexpr uint32 I = 0;  //max index-placeholder count of sub-expr(e.g. 2_ )

		value_type t;

		CCDK_FORCEINLINE constexpr explicit
			expr(T&& inT)
			:t(inT)
		{
			static_assert(!is_rref_v< decltype(inT) >,
				"reference expr can't be temporary object");
		}


		CCDK_FORCEINLINE constexpr explicit
		operator value_type() const noexcept
		{
			return t;
		}

		template<uint32 Start, typename... Args>
		CCDK_FORCEINLINE
		const T&
		eval(Args&&...) const noexcept
		{
			return t;
		}

		template<typename... Args>
		CCDK_FORCEINLINE constexpr
		value_type&
		operator()(Args&&... args) noexcept
		{
			return t;
		}
	};

	template<typename T>
	using ref_t = expr< reference_t<T> >;

	constexpr create_raw_t<ref_t> ref{};
	constexpr create_const_raw_t<ref_t> cref{};

ccdk_namespace_mpl_fn_end