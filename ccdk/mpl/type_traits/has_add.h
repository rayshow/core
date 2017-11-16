#pragma once


#include<ccdk/mpl/base/and_.h>
#include<ccdk/mpl/base/not_.h>
#include<ccdk/mpl/type_traits/is_pointer.h>
#include<ccdk/mpl/type_traits/is_integer.h>
#include<ccdk/mpl/type_traits/impl/has_operator_decl.h>

namespace ccdk
{
	namespace mpl
	{
		CCDK_TT_HAS_NO_SIDE_EFFECT_BINARY_OP(+, add);

		//clang T + P* will not report error
		template<typename L, typename R>
		struct add_assign_clang_fix :not_< and_< is_integer<L>, is_pointer<R> >> {};
                
		namespace operator_detail                                                    
		{                                                                            
			template<typename L, typename R>                                         
			struct has_add_assign_helper                                          
			{                                                                        
				template<typename U, typename P,                                    
				typename = decltype(makeval<U>() += declval<P>())>               
					static constexpr bool sfinae(int) { return true; }               
				template<typename U, typename P>                                     
				static constexpr bool sfinae(...) { return false; }                  
				static constexpr bool value = sfinae<L, R>(0);                       
			};                                                                       
		}                                                                            
		template<typename L, typename R = L >                                        
		struct has_add_assign: and_< add_assign_clang_fix<L,R>, operator_detail::has_add_assign_helper<L, R>> {};
		template<typename L, typename R = L>                                         
		static constexpr bool has_add_assign_v = has_add_assign<L,R>::value;

		
	} 
}