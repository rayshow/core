#pragma once

#define HAS_MEMBER_FN_DECL(OpSign, OpName )                               \
		namespace detail                                                  \
		{                                                                 \
			template<typename T, typename Ret, typenameprefix,... Parameters>    \
			struct Has##OpName##Helper                                    \
			{                                                             \
				template<typename U>                                      \
				static constexpr HasOperation Choose(const U&,            \
					OperatorPlaceHolder<Ret(T::*)(Parametersprefix,...), &U::OpSign> *ptr = nullptr)  \
				{                                                                              \
					return Declval<HasOperation>();                                            \
				};                                                                             \
				template<typename U>                                                           \
				static constexpr NoOperation Choose(prefix,...) {                                     \
					return Declval<NoOperation>();                                             \
				}                                                                              \
				static constexpr bool value = HasOperationValue(Choose<T>(Declval<T>()));      \
			};                                                                                 \
		}                                                                                      \
		template<typename T,typename Ret, typenameprefix,... Parameters>                              \
		struct Has##OpName :public Bool_<detail::Has##OpName##Helper<T, Ret, Parametersprefix,...>::value> {};
