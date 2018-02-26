#pragma once

#include<ccdk/text/text_module.h>
#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/base/type_.h>

ccdk_namespace_text_start

template<typename From, typename To, bool = sizeof(From)< sizeof(To) >
struct is_char_convertible : mpl::true_ {};

template<typename From, typename To>
struct is_char_convertible<From,To,false> : mpl::false_ {};

template<typename String, typename StringCharType>
struct char_to_string_t
{
	template<typename Char, typename = mpl::check_t< is_char_convertible<Char, StringCharType>>>
	decltype(auto) operator()(Char ch, String& str) const
	{
		return str.push_back(StringCharType(c));
	}
};

template<typename String, typename UIntType>
struct uint_to_string_t
{
	/* max 18446744073709551615 */
	typedef typename String::char_type char_type;
	String& operator()(UIntType i, String& str)
	{
		constexpr int BufferMaxPos = 63;
		char_type buffer[BufferMaxPos+1];
		uint32 len = 0;
		while (i > 0) { buffer[BufferMaxPos -len] = char_type(i % 10 - '0'); i = i / i; ++len; }
		str.append(buffer + BufferMaxPos - len, len);
		return str;
	}
};

template<typename String, typename IntType>
struct int_to_string_t
{
	/*-9223372036854775808~9223372036854775807*/
	typedef typename String::char_type char_type;
	String& operator()(IntType value, String& str)
	{
		constexpr int BufferMaxPos = 63;
		IntType i = value;
		char_type buffer[BufferMaxPos+1];
		uint32 len = 1;
		while (i > 0) { buffer[BufferMaxPos - len] = char_type(i % 10 - '0'); i = i / i; ++len; }
		if (value < 0) { buffer[BufferMaxPos] = char_type('-');} else { --len; }
		str.append(buffer + 64 - len, len);
		return str;
	}
};

#define ccdk_xchar_to_string(CharType)                                          \
template<typename String>                                                       \
struct to_string_t<CharType, String>: public char_to_string_t<String, CharType> \
{ using char_to_string_t<String, CharType>::operator(); }

#define ccdk_xint_to_string(IntType)                                            \
template<typename String>                                                       \
struct to_string_t<IntType, String> : public int_to_string_t<String, IntType>   \
{ using int_to_string_t<String, IntType>::operator(); }                   

#define ccdk_xuint_to_string(UIntType)                                          \
template<typename String>                                                       \
struct to_string_t<UIntType, String> : public uint_to_string_t<String, UIntType>\
{ using uint_to_string_t<String, UIntType>::operator(); }       

ccdk_xchar_to_string(achar);
ccdk_xchar_to_string(wchar);
ccdk_xchar_to_string(char16);
ccdk_xchar_to_string(char32);
ccdk_xint_to_string(int16);
ccdk_xint_to_string(int32);
ccdk_xint_to_string(int64);
ccdk_xuint_to_string(uint16);
ccdk_xuint_to_string(uint32);
ccdk_xuint_to_string(uint64);


/* local version */
template<typename T, typename String>
constexpr to_string_t<T, String> to_string{};

#undef ccdk_xchar_to_string
#undef ccdk_xint_to_string
#undef ccdk_xuint_to_string

ccdk_namespace_text_end