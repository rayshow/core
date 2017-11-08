#pragma once

#include"../common_def.h"

namespace Aurora3D
{
	//sizeof<T> is real data size of T, no margin or pointer
	template<typename T> struct IsRealSizeType { enum { value = false }; };
	template<> struct IsRealSizeType<bool>     { enum{ value = true }; };
	template<> struct IsRealSizeType<float>    { enum{ value = true }; };
	template<> struct IsRealSizeType<double>   { enum{ value = true }; };
	template<> struct IsRealSizeType<uint8>    { enum{ value = true }; };
	template<> struct IsRealSizeType<uint16>   { enum{ value = true }; };
	template<> struct IsRealSizeType<uint32>   { enum{ value = true }; };
	template<> struct IsRealSizeType<uint64>   { enum{ value = true }; };
	template<> struct IsRealSizeType<int8>     { enum{ value = true }; };
	template<> struct IsRealSizeType<int16>    { enum{ value = true }; };
	template<> struct IsRealSizeType<int32>    { enum{ value = true }; };
	template<> struct IsRealSizeType<int64>    { enum{ value = true }; };
	template<> struct IsRealSizeType<achar>    { enum{ value = true }; };
	template<> struct IsRealSizeType<wchar>    { enum{ value = true }; };

	//Serializable T need to be RealSizeType or string or some-self-defined-type which marked true
	template<typename T> struct IsSerializable { enum { value = IsRealSizeType<T>::value }; };
	template<> struct IsSerializable<AString>  { enum{ value = true }; };
	template<> struct IsSerializable<WString>  { enum{ value = true }; };
}