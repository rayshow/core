#pragma once

#include<ccdk/text/text_module.h>
#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/base/type_.h>

ccdk_namespace_text_start

//append uint16/uint32/uint64 to string
template<typename String, typename UIntType>
struct uint_to_string_t
{
	using char_type = typename String::char_type;

	// uint64 range from 0 to 18446744073709551615 
	String& operator()(UIntType val, String& str) const {
		constexpr int kBufferMax = 64;
		constexpr int kBufferEnd = 65;
		char_type buffer[kBufferEnd];
		uint32 len = 0;
		while (val > 0) {
			buffer[kBufferMax - len] = char_type(val % 10) + '0';
			val = val / 10;
			++len;
		}
		str.append(buffer + kBufferEnd - len, len);
		return str;
	}
};

//append int16/int32/int64 to string
template<typename String, typename IntType>
struct int_to_string_t
{
	// int64 range from -9223372036854775808 to 9223372036854775807
	using char_type = typename String::char_type;
	String& operator()(IntType i, String& str) const {
		constexpr int kBufferMax = 64;
		constexpr int kBufferEnd = 65;
		IntType val = i;
		if(i<0) val =-val;
		char_type buffer[kBufferEnd];
		uint32 len = 0;
		while (val > 0) {
			buffer[kBufferMax - len] = char_type(val % 10) + '0';
			val = val / 10;
			++len;
		}
		if (i < 0) {
			buffer[kBufferMax -len] = char_type('-');
			++len;
		}
		str.append(buffer + 65 - len, len);
		return str;
	}
};

#define ccdk_xint_to_string(IntType)                                            \
template<typename String>                                                       \
struct to_string_t<IntType, String> : public int_to_string_t<String, IntType>   \
{ using int_to_string_t<String, IntType>::operator(); }

#define ccdk_xuint_to_string(UIntType)                                          \
template<typename String>                                                       \
struct to_string_t<UIntType, String> : public uint_to_string_t<String, UIntType>\
{ using uint_to_string_t<String, UIntType>::operator(); }

ccdk_xint_to_string(int16);
ccdk_xint_to_string(int32);
ccdk_xint_to_string(int64);
ccdk_xuint_to_string(uint16);
ccdk_xuint_to_string(uint32);
ccdk_xuint_to_string(uint64);

//simi-function 
template<typename T, typename String>
constexpr to_string_t<T, String> to_string{};

#undef ccdk_xint_to_string
#undef ccdk_xuint_to_string

ccdk_namespace_text_end