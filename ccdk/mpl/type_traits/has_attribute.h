#pragma once

#include<ccdk/mpl/type_traits/impl/has_attribute_decl.h>

ccdk_namespace_mpl_start

// test wheather T().next is well-formed and is type T*
CCDK_TT_HAS_ATTRIBUTE_DECL1(next);

// test wheather T().prev is well-formed and is type T*
CCDK_TT_HAS_ATTRIBUTE_DECL1(prev);


ccdk_namespace_mpl_end