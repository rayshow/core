#pragma once

#include<ccdk/mpl/base/bool_.h>

namespace ccdk
{
	namespace mpl
	{
		//ingore const volatile
		template<typename A,typename B> struct is_same : public false_ {};
		template<typename T> struct is_same<T, T> :public true_ {};
		template<typename A,typename B> constexpr bool is_same_v = is_same<A, B>::value;

#define AssertTypeSame(A,B)  static_assert(is_same_v<A,B>,"")
#define AssertTypeDiff(A,B) static_assert(!is_same_v<A,B>,"")
	}
}