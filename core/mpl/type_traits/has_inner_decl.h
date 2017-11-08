#pragma once

#define A3D_MPL_HAS_INNER_DECL(Name, InnerType) \
	namespace detail                         \
	{                                        \
		template<typename T>                 \
		struct HasInner ## Name ## Helper    \
		{                                    \
			template<typename U>             \
			static  HasOperation Test(U *t1, typename U::## InnerType ## *t2 = 0);  \
			static  NoOperation  Test(prefix,...);                                    \
			static constexpr bool value = HasOperationValue(Test(static_cast<T*>(0))); \
		};                                   \
	}                                        \
	template<typename T> struct HasInner ## Name:public Bool_<detail::HasInner ## Name ## Helper<T>::value>{}; 
