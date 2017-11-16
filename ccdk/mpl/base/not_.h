#pragma once

#include<ccdk/mpl/base/bool_.h>

namespace ccdk
{
	namespace mpl
	{
		template<typename T> struct not_ : public bool_< !T::value > {};


	}
}