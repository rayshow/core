#pragma once

#include<ccdk/mpl/if.h>

namespace Aurora3D
{
	namespace mpl
	{
		// for non-inner-contained-type type
		// if(B1)      type = Case2
		// else if(B2) type = Case1
		// else        type = default 
		template<bool B1, bool B2, typename Case1, typename Case2, typename Default>
		struct BoolChoose3 { typedef Default type; };

		template<typename Case1, typename Case2, typename Default>
		struct BoolChoose3<true, false, Case1, Case2, Default> { typedef Case1 type; };

		template<typename Case1, typename Case2, typename Default>
		struct BoolChoose3<false, true, Case1, Case2, Default> { typedef Case2 type; };

		// if(C::value) type = T1
		// else type = T2
		template<typename B1, typename B2, typename Case1, typename Case2, typename Default>
		struct Choose3 :public BoolChoose3<B1::value, B2::value, Case1, Case2, Default> {};

		// for inner-contained-type type
		// if(B1)       derive Case2
		// else if(B2)  derive Case1
		// else         derive Default
		template<bool B1, bool B2, typename Case1, typename Case2, typename Default>
		struct BoolDeriveChoose3:public Default {};

		template<typename Case1, typename Case2, typename Default>
		struct BoolDeriveChoose3<true, false, Case1, Case2, Default>:public Case1 {};

		template<typename Case1, typename Case2, typename Default>
		struct BoolDeriveChoose3<false, true, Case1, Case2, Default>:public Case2 {};

		// if(C::value) derive T1
		// else derive T2
		template<typename B1, typename B2, typename Case1, typename Case2, typename Default>
		struct DeriveChoose3 :public BoolDeriveChoose3<B1::value, B2::value, Case1, Case2, Default> {};
	}
}