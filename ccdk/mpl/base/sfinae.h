#pragma once

#include<ccdk/mpl/base/bool_.h>

namespace ccdk
{
	namespace mpl
	{

		template<typename... Args> struct sfinae:true_ {};

	}
}