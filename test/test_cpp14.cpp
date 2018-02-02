#include<cstdio>
#include<utility>
#include<memory>
#include<string>
#include<typeinfo>
#include<iostream>
#include<type_traits>
#include<cassert>
#include<functional>
#include<ccdk/mpl/base/arithmetic_.h>
#include<ccdk/mpl/base/placeholder.h>
#include<ccdk/debug_type.hpp>

using namespace ccdk::mpl;
using namespace ccdk;


int main()
{
	int v;
	DebugValue(v = add2_< int32_<1>, int32_<2> >::value);
	DebugValue(v = add_v< int32_<1>, int32_<2> >);
	DebugValue(v = add_cv< int, 1,2 >);

	DebugValue(v = sub2_< int32_<1>, int32_<2> >::type::value);
	DebugValue(v = div2_< int32_<1>, int32_<2> >::type::value);
	DebugValue(v = mul2_< int32_<1>, int32_<2> >::type::value);
	DebugValue(v = mod2_< int32_<1>, int32_<2> >::type::value);
	DebugValue(v = shl2_< int32_<1>, int32_<2> >::type::value);
	DebugValue(v = shr2_< int32_<1>, int32_<2> >::type::value);
	DebugValue(v = reverse_< int32_<1> >::value);
	DebugValue(v = reverse_< int32_<1> >::value);
	
		 
	getchar();
	return 0;
}