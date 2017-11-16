#pragma once

#include<ccdk/mpl/base/bool_.h>

namespace ccdk
{
	namespace mpl
	{
		struct null_ {};
		template<typename T> struct is_null :public false_ {};
		template<> struct is_null< null_> : public true_ {};
	}
}