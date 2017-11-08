#pragma once

#include<core/mpl/bool_.h>

namespace Aurora3D
{
	namespace mpl
	{
		// NOT standard layout
		//1. object member have different access control  
		//2. have virtual function or virtual base classes
		//3. have reference object member 
		//4. have empty base class as the first object member
		//5. have base class owned object member
		//6. have diamond derive class
		//7. data member and base class are NOT standard layout
		template<typename T> struct IsStandLayout :public Bool_<__is_standard_layout(T)> {};
		template<typename T> using IsStandLayout_t = typename IsStandLayout<T>::type;
#define IsStandLayoutV(T)   (IsStandLayout<T>::value)
#define NotStandLayoutV(T)  (!IsStandLayout<T>::value)
	}
}