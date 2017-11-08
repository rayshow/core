#pragma once

namespace Aurora3D
{
	namespace mpl
	{ 
		//for type trait
		template<typename It1, typename It2> struct Distance 
			:public It1::template distance<It2> {};
	}
}