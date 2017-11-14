#pragma once 

#include<core/mpl/base/bool_.h>
#include<core/mpl/type_traits/declval.h>

namespace core
{
	namespace mpl
	{
		namespace detail                                                  
		{                                                                 
			template<typename T, typename Ret>    
			struct has_arrow_helper
			{                                                             
				template<typename U, typename = fn_sfinae<Ret(U::*)(), &U:: operator-> > >
				static constexpr bool sfinae(int) {  return true; };          

				template<typename U>  static constexpr bool sfinae(...) { return false; }

				static constexpr bool value = sfinae<T>(0);
			};                                                                                 
		}                                             

		template<typename T,typename Ret>                              
		struct has_arrow :public bool_<detail::has_arrow_helper<T, Ret>::value> {};

		template<typename T, typename Ret>
		static constexpr bool has_arrow_v = has_arrow<T,Ret>::value;
	}
}