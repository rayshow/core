#pragma once

#include<core/mpl/base/bool_.h>
#include<core/mpl/type_traits/declval.h>
#include<core/mpl/type_traits/impl/has_operator_type.h>

#define HAS_NEW_DELETE_DECL(sign, opname, ...)                                                \
		namespace  detail                                                                     \
		{                                                                                     \
			template<typename T, typename... Args>                                            \
			struct has_ ## opname ## _helper                                                  \
			{                                                                                 \
				template<typename U>                                                          \
				static constexpr has_op choose(const U&,                                      \
					fn_sfinae<void*(*)(Args...), &U::sign> *ptr = nullptr)                    \
				{                                                                             \
					return declval<has_op>();                                                 \
				};                                                                            \
				template<typename U>                                                          \
				static constexpr no_op choose(Args...) {                                      \
					return declval<no_op>();                                                  \
				}                                                                             \
				static constexpr bool value = A3D_TT_HAS_OP( choose<T>(declval<T>()) );       \
			};                                                                                \
		}                                                                                     \
		template<typename T>                                                                  \
		struct has_ ## opname:public bool_<detail::has_ ## opname ## _helper<T, __VA_ARGS__>::value>{}; \
		template<typename T>                                                                  \
		using has_ ## opname ## _v = has_ ## opname<T>::value;