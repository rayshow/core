#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/type_traits/is_rref.h>
#include<ccdk/mpl/type_traits/remove_const.h>
#include<ccdk/mpl/util/move.h>
#include<ccdk/mpl/util/create.h>
#include<ccdk/mpl/function/function_fwd.h>

ccdk_namespace_mpl_fn_start

	template<typename T>
	struct expr< reference_t<T> >
	{
		typedef expr type;
		typedef T& value_type;
		static constexpr uint32 size = 0;	//args length
		static constexpr uint32 wild_size = 0;  //wild placeholder count of sub-expr(e.g. _ )
		static constexpr uint32 index_size = 0;  //max index-placeholder count of sub-expr(e.g. 2_ )

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

	constexpr util::create_t<ref_t> ref{};
	constexpr util::create_const_t<ref_t> cref{};

ccdk_namespace_mpl_fn_end