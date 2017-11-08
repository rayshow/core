#pragma once

#include<core/mpl/bool_.h>
#include<core/type.h>

namespace Aurora3D
{   
	namespace mpl
	{
		template<typename T> struct IsIntegral :public False_ {};
		template<> struct IsIntegral<bool>   :public True_ {};
		template<> struct IsIntegral<achar>  :public True_ {};
		template<> struct IsIntegral<wchar>  :public True_ {};
		template<> struct IsIntegral<char16> :public True_ {};
		template<> struct IsIntegral<char32> :public True_ {};
		template<> struct IsIntegral<int8>   :public True_ {};
		template<> struct IsIntegral<int16>  :public True_ {};
		template<> struct IsIntegral<int32>  :public True_ {};
		template<> struct IsIntegral<int64>  :public True_ {};
		template<> struct IsIntegral<uint8>  :public True_ {};
		template<> struct IsIntegral<uint16> :public True_ {};
		template<> struct IsIntegral<uint32> :public True_ {};
		template<> struct IsIntegral<uint64> :public True_ {};
		template<> struct IsIntegral<long> :public True_ {};
		template<> struct IsIntegral<unsigned long> :public True_ {};
		template<typename T> struct IsIntegral<const T>: public IsIntegral<T>{};
		template<typename T> struct IsIntegral<volatile T> : public IsIntegral<T> {};
		template<typename T> struct IsIntegral<const volatile T> : public IsIntegral<T> {};

#define IsIntegralV(T)   (IsIntegral<T>::value)
#define NotIntegralV(T)  (!IsIntegral<T>::value)
	}
}