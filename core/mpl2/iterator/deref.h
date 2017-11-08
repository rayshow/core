#pragma once

namespace Aurora3D
{
	namespace mpl
	{
		//for type trait
		template<typename It> struct Deref :public It::deref {};
	}
}