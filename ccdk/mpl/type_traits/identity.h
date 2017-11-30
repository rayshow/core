#pragma once

namespace ccdk
{
	namespace mpl
	{
		template<typename T>
		struct identity
		{
			typedef T type;
		};
	}
}