#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/base/integer_.h>

ccdk_namespace_mpl_start

template<bool v> struct bool_ :public compile_t<bool, v>
{
	typedef bool_<v> type;
};

template<bool v> bool_<v> bool_c{};

using false_ = bool_<false>;
using true_ = bool_<true>;
		
constexpr false_ false_c{};
constexpr true_  true_c{};

ccdk_namespace_mpl_end