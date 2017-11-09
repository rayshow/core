#pragma once

#include<core/mpl/base/bool_.h>

namespace core
{
	namespace mpl
	{
		template<typename T> struct not_ : public bool_< !T::value > {};


	}
}