#pragma once
namespace ccdk
{
	namespace mpl
	{
#define BoolIfV(c,a,b)   ((c)?(a)::(b))

		// for non-inner-contained-type type
		// if(B) type = T1
		// else type = T2
		template<bool B, typename T1, typename T2> struct bool_if  { typedef T1 type; };
		template<typename T1, typename T2> struct bool_if<false, T1, T2>  { typedef T2 type; };

		template<bool B, typename T1, typename T2> using bool_if_t = typename bool_if<B, T1, T2>::type;

		// if(C::value) type = T1
		// else type = T2
		template<typename C, typename T1, typename T2> struct if_ :public bool_if<C::value, T1, T2> {};
		template<typename C, typename T1, typename T2> using if_t = typename if_<C, T1, T2>::type;

	}
}