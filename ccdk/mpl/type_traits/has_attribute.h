#pragma once

#include<ccdk/mpl/type_traits/impl/has_attribute_decl.h>

ccdk_namespace_mpl_start

// test wheather T().next is well-formed and is type T*
CCDK_TT_HAS_ATTRIBUTE_DECL1(next);

// test wheather T().prev is well-formed and is type T*
CCDK_TT_HAS_ATTRIBUTE_DECL1(prev);

// test wheather T().parent is well-formed and is type T*
CCDK_TT_HAS_ATTRIBUTE_DECL1(parent);

// test wheather T().left is well-formed and is type T*
CCDK_TT_HAS_ATTRIBUTE_DECL1(left);

// test wheather T().right is well-formed and is type T*
CCDK_TT_HAS_ATTRIBUTE_DECL1(right);

ccdk_namespace_mpl_end