#pragma once

namespace Aurora3D
{
	namespace mpl
	{
		template<typename S> struct Begin:public S::begin {};
	}
}