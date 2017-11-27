#pragma once
namespace ccdk
{
	namespace mpl
	{
		// if(B) derive T1
		// else  derive T2
		template<bool B, typename T1, typename T2> struct bool_condi_derive:public T1 {};
		template<typename T1, typename T2>         struct bool_condi_derive<false, T1, T2> :public T2 {};

		// get condi_derive<B,T1,T2>::type
		template<bool B, typename T1, typename T2> using bool_condi_derive_t = typename bool_condi_derive<B, T1, T2>::type;

		// if(C::value) derive T1
		// else derive T2
		template<typename C, typename T1, typename T2>
		struct condi_derive :public bool_condi_derive<C::value, T1, T2> {};

		template<typename T>
		struct identity
		{
			typedef T type;
		};
	}
}