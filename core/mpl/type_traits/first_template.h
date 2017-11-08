#pragma once

#include<core/preprocessor/seq_foreach_item.h>

namespace Aurora3D
{
	namespace mpl
	{
		template<typename T> struct FirstTemplateType { typedef T type; };

#define FIRST_TEMPLATE_TYPE(TemplateType, prefix,...)                                                      \
		template<typename T> struct TemplateType;                                                   \
		template<typename T> struct FirstTemplateType< TemplateType<T> > :public FirstTemplateType<T> {};

#define TEMPLATE_SET  (add_const, AddCV, AddLValueRef, AddRValueRef, AddPointer, AddUnsigned, AddVolatile,\
                AddSigned, AddConstLRef, AddTopConst, ConvertConstLRef,\
				RemoveAllDim, RemoveConst, RemoveCV, RemoveDim, RemovePointer, RemoveRef, RemoveTopConst,\
				RemoveVolatile, UnderlyingType, Decay)
		A3D_PP_FOREACH_ITEM(FIRST_TEMPLATE_TYPE, TEMPLATE_SET);
#undef FIRST_TEMPLATE_TYPE
#undef TEMPLATE_SET
	}
}