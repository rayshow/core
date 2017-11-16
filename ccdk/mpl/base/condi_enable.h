#pragma once

namespace ccdk
{
	namespace mpl
	{
		//if true, get type, otherwise get compile error
		template<bool B, typename T> struct true_enable {  };
		template<typename T> struct true_enable<true,T> { typedef T type; };

		//if false, get type
		template<bool B, typename T> struct false_enable {};
		template<typename T> struct false_enable<false, T> { typedef T type; };

		template<bool B,typename T> 
		using true_enable_t = typename true_enable<B, T>::type;

		template<bool B, typename T>
		using false_enable_t = typename false_enable<B, T>::type;
	}
}