#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/base/integer_.h>

ccdk_namespace_mpl_start

template<char c> using achar_ = compile_t<achar, c>;
template<char c> constexpr achar_<c> char_c{};

template<wchar_t c> using wchar_ = compile_t<wchar, c>;
template<wchar_t c> constexpr wchar_<c> wchar_c{};

template<wchar_t c> using char16_ = compile_t<char16, c>;
template<wchar_t c> constexpr char16_<c> char16_c{};

template<wchar_t c> using char32_ = compile_t<char32, c>;
template<wchar_t c> constexpr char32_<c> char32_c{};


template<typename T>
struct chars_ {};

template<>
struct chars_<achar>
{
	static constexpr achar end  = '\0';
	static constexpr achar newline = '\n';
};

ccdk_namespace_mpl_end