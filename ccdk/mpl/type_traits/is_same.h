#pragma once

#include<ccdk/mpl/base/type_.h>

namespace ccdk
{
	namespace mpl
	{
		namespace tt_impl
		{

			//for some special class 
			template<typename T1, typename T2> struct is_same_impl :false_ {};
		}

		//ingore const volatile
		template<typename A,typename B> struct is_same : public tt_impl::is_same_impl<A,B> {};
		template<typename T> struct is_same<T, T> :public true_ {};
		template<typename A,typename B> constexpr bool is_same_v = is_same<A, B>::value;

#define AssertTypeSame(A,B)  static_assert(is_same_v<A,B>,"")
#define AssertTypeDiff(A,B) static_assert(!is_same_v<A,B>,"")
	}
}