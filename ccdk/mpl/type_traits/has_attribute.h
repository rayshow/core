#pragma once

#include<ccdk/mpl/type_traits/impl/has_attribute_decl.h>

ccdk_namespace_mpl_start

// test wheather T().next is well-formed and is type T*
CCDK_TT_HAS_ATTRIBUTE_WITH_TYPE_DECL(has_attribute_next, next, T*);

// test wheather T().prev is well-formed and is type T*
CCDK_TT_HAS_ATTRIBUTE_WITH_TYPE_DECL(has_attribute_prev, prev, T*);

// test wheather T().parent is well-formed and is type T*
CCDK_TT_HAS_ATTRIBUTE_WITH_TYPE_DECL(has_attribute_parent, parent, T*);

// test wheather T().left is well-formed and is type T*
CCDK_TT_HAS_ATTRIBUTE_WITH_TYPE_DECL(has_attribute_left, left, T*);

// test wheather T().right is well-formed and is type T*
CCDK_TT_HAS_ATTRIBUTE_WITH_TYPE_DECL(has_attribute_right, right, T*);

ccdk_namespace_mpl_end