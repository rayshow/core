#pragma once
namespace ccdk
{
	namespace mpl
	{
		// if(B) derive T1
		// else  derive T2
		template<bool B, typename T1, typename T2> struct derive_if_c:public T1 {};
		template<typename T1, typename T2>         struct derive_if_c<false, T1, T2> :public T2 {};

		// get condi_derive<B,T1,T2>::type
		template<bool B, typename T1, typename T2> 
		using derive_if_t = typename derive_if_c<B, T1, T2>::type;

		// if(C::value) derive T1
		// else derive T2
		template<typename C, typename T1, typename T2>
		struct derive_if :public derive_if_c<C::value, T1, T2> {};

	}
}