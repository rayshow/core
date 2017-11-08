#pragma once 

#include<core/mpl/type_traits/has_operator_type.h>

namespace Aurora3D
{
	namespace mpl
	{

		namespace detail                                                  
		{                                                                 
			template<typename T, typename Ret>    
			struct HasReferArrowHelper
			{                                                             
				template<typename U>                                      
				static constexpr HasOperation Choose(const U&,            
					OperatorPlaceHolder<Ret(T::*)(), &U::operator-> > *ptr = nullptr)  
				{                                                                              
					return Declval<HasOperation>();                                            
				};                                                                             
				template<typename U>                                                          
				static constexpr NoOperation Choose(prefix,...) {                                     
					return Declval<NoOperation>();                                             
				}                                                                              
				static constexpr bool value = HasOperationValue(Choose<T>(Declval<T>()));      
			};                                                                                 
		}                                                                                      
		template<typename T,typename Ret, typenameprefix,... Parameters>                              
		struct HasReferArrow :public Bool_<detail::HasReferArrowHelper<T, Ret>::value> {};
	}
}