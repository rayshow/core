#pragma once


#define HAS_NEW_DELETE_DECL(OpSign, OpName, prefix,...)              \
		namespace  detail                                     \
		{                                                     \
			template<typename T, typenameprefix,... Parameters>      \
			struct Has##OpName##Helper                        \
			{                                                 \
				template<typename U>                          \
				static constexpr HasOperation Choose(const U&,                                \
					OperatorPlaceHolder<void*(*)(Parametersprefix,...), &U::OpSign> *ptr = nullptr)  \
				{                                                                             \
					return Declval<HasOperation>();                                           \
				};                                                                            \
				template<typename U>                                                          \
				static constexpr NoOperation Choose(prefix,...) {                                    \
					return Declval<NoOperation>();                                            \
				}                                                                             \
				static constexpr bool value = HasOperationValue( Choose<T>(Declval<T>()) );   \
			};                                                                                \
		}                                                                                     \
		template<typename T>                                                                  \
		struct Has##OpName:public Bool_<detail::Has##OpName##Helper<T, __VA_ARGS__>::value>{};