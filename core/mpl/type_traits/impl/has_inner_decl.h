#pragma once

#include<core/mpl/type_traits/declval.h>
#include<core/mpl/type_traits/impl/has_operator_type.h>


#define A3D_TT_HAS_INNER_DECL(name, inner_name)                                        \
	namespace detail                                                                   \
	{                                                                                  \
		template<typename T>                                                           \
		struct has_inner_ ## name ## _helper                                           \
		{                                                                              \
			template<typename U>                                                       \
			constexpr static  has_op test(U *t1, typename U::## inner_name ## *t2 = 0) \
			{                                                                          \
				return declval<has_op>();                                              \
			};                                                                         \
			constexpr static  no_op  test(...){ return declval<no_op>(); };            \
			static constexpr bool value = A3D_TT_HAS_OP(test(static_cast<T*>(0)));     \
		};                                                                             \
	}                                                                                  \
	template<typename T> struct has_inner_ ## name:                                    \
		public bool_< detail::has_inner_ ## name ## _helper<T>::value >{};             \
	template<typename T>                                                               \
		static constexpr bool has_inner_ ## name ## _v = has_inner_ ## name<T>::value;
