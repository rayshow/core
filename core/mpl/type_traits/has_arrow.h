#pragma once 

#include<core/mpl/base/bool_.h>
#include<core/mpl/type_traits/declval.h>
#include<core/mpl/type_traits/impl/has_operator_type.h>

namespace core
{
	namespace mpl
	{
		namespace detail                                                  
		{                                                                 
			template<typename T, typename Ret>    
			struct has_arrow_helper
			{                                                             
				template<typename U>                                      
				static constexpr has_op choose(const U&,            
					fn_sfinae<Ret(U::*)(), &U::operator-> > *ptr = nullptr)  
				{                                                                              
					return declval<has_op>();                                            
				};                                                                             
				template<typename U>                                                          
				static constexpr no_op choose(...) {
					return declval<no_op>();
				}                                                                              
				static constexpr bool value = A3D_TT_HAS_OP(choose<T>(declval<T>()));
			};                                                                                 
		}                                             

		template<typename T,typename Ret>                              
		struct has_arrow :public bool_<detail::has_arrow_helper<T, Ret>::value> {};

		template<typename T, typename Ret>
		static constexpr bool has_arrow_v = has_arrow<T,Ret>::value;
	}
}