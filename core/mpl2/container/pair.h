#pragma once


namespace Aurora3D
{
	namespace mpl
	{
		template<typename T1, typename T2>
		struct Pair
		{
			typedef T1 first;
			typedef T2 second;
		};
	}
}