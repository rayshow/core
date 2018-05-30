#pragma once

#include<ccdk/text/text_module.h>
#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/base/type_.h>

ccdk_namespace_text_start

template<typename from, typename to, bool = (sizeof(from) < sizeof(to)) >
struct is_char_convertible : mpl::true_ {};

template<typename from, typename to>
struct is_char_convertible<from, to, false> : mpl::false_ {};


ccdk_namespace_text_end
