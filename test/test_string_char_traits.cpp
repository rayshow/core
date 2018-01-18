#include<cstdio>
#include<utility>
#include<memory>
#include<string>
#include<typeinfo>
#include<iostream>
#include<type_traits>
#include<cassert>
#include<functional>
#include<ccdk/string/char_traits.h>
#include<ccdk/mpl/base/integer_.h>

using namespace ccdk;
using namespace ccdk::str;
using namespace ccdk::mpl;

template<typename T>
struct test_tmpl
{
	test_tmpl() = default;

	template<typename U>
	test_tmpl(const test_tmpl<U>& tmp) { DebugValue("tmpl constructor"); }

	template<typename U>
	void test(const test_tmpl<U>& tmp) { DebugValue("tmpl test"); }

};

int main()
{
	DebugValue(size_c< common_char_traits<achar>::strlen(u8"abc") >);
	DebugValue(size_c< common_char_traits<wchar>::strlen(L"abc") >);
	DebugValue(size_c< common_char_traits<char16>::strlen(u"abc") >);
	DebugValue(size_c< common_char_traits<char32>::strlen(U"abc") >);

	char cs[5] = u8"";
	wchar ws[4];
	char16 c16s[4];
	char32 c32s[4];
	DebugValue( common_char_traits<achar>::strcpy(cs, u8"ab") );


	getchar();
	return 0;
}