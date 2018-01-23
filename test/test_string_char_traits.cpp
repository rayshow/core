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
#include<ccdk/mpl/util/move.h>
#include<vector>

using namespace ccdk;
using namespace ccdk::str;
using namespace ccdk::mpl;

int main()
{
	//                   01234567890123456789012
	std::string source{ "BBC ABCDAB ABCDABCDABDE" };
	std::string search{ "ABD" };
	ptr::size_t pos = str::char_traits<char>::find(source.c_str(), source.length(), search.c_str(), search.length());
	DebugValue(pos);
	getchar();
	return 0;
}