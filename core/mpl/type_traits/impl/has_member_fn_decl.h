#pragma once

#include<core/mpl/type_traits/declval.h>
#include<core/mpl/type_traits/impl/has_operator_type.h>

#define HAS_MEMBER_FN_DECL(sign, opname )                                                      \
		namespace detail                                                                       \
		{                                                                                      \
			template<typename T, typename Ret, typename... Args>                               \
			struct has_ ## opname ## _helper                                                   \
			{                                                                                  \
				template<typename U>                                                           \
				static constexpr has_op choose(const U&,                                       \
					fn_sfinae<Ret(T::*)(Args...), &U::sign> *ptr = nullptr)                    \
				{                                                                              \
					return declval<has_op>();                                                  \
				};                                                                             \
				template<typename U>                                                           \
				static constexpr no_op choose(...) {                                           \
					return declval<no_op>();                                                   \
				}                                                                              \
				static constexpr bool value = A3D_TT_HAS_OP(choose<T>(declval<T>()));          \
			};                                                                                 \
		}                                                                                      \
		template<typename T,typename Ret, typename... Args>                                    \
		struct has_ ## opname :public bool_< detail::has_ ## opname ## _helper <T, Ret, Args...>::value > {}; \
		template<typename T, typename Ret, typename... Args>                                   \
		using has_ ## opname ## _v = has_ ## opname<T, Ret, Args...>::value;
