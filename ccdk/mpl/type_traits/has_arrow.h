#pragma once 

#include<ccdk/mpl/base/bool_.h>
#include<ccdk/mpl/base/logic_.h>
#include<ccdk/mpl/type_traits/declval.h>
#include<ccdk/mpl/type_traits/remove_pointer.h>
#include<ccdk/mpl/type_traits/is_class.h>
#include<ccdk/mpl/type_traits/is_union.h>

namespace ccdk
{
	namespace mpl
	{
		namespace tt_detail
		{   
			template<typename T, typename Ret>    
			struct is_arrow_well_formed_helper
			{                                                             
				template<typename U, typename = fn_sfinae<Ret(U::*)(), &U:: operator-> > >
				static constexpr bool sfinae(int) {  return true; };          

				template<typename U>  static constexpr bool sfinae(...) { return false; }

				static constexpr bool value = sfinae<T>(0);
			};

			template<typename T, typename Ret>
			struct is_arrow_well_formed :bool_<is_arrow_well_formed_helper<T, Ret>::value> {};

			//check ret is class or union pointer
			template<typename Ret, typename ret_noptr_t = remove_pointer_t<Ret>>
			struct is_arrow_ret_valid:or_< is_class< ret_noptr_t>, is_union<ret_noptr_t> >{};
		}                                             

		//operator->() need return class or union pointer, and T::operator->() need be well-formed
		template<typename T,typename Ret>                              
		struct has_arrow :public and_<  tt_detail::is_arrow_ret_valid<Ret>, tt_detail::is_arrow_well_formed<T, Ret>> {};

		template<typename T, typename Ret>
		static constexpr bool has_arrow_v = has_arrow<T,Ret>::value;
	}
}