#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/base/bool_.h>

ccdk_namespace_mpl_start
		
struct null_ {};
template<typename T> struct is_null :public false_ {};
template<> struct is_null< null_> : public true_ {};

ccdk_namespace_mpl_end