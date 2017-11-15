#pragma once

#include<core/mpl/base/bool_.h>
#include<core/mpl/base/size_.h>
#include<core/mpl/type_traits/impl/has_operator_decl.h>

namespace core
{
	namespace mpl
	{
		/*
		* new T results in a call of operator new(sizeof(T)),
		* new(2, f) T results in a call of operator new(sizeof(T), 2, f),
		* new T[5] results in a call of operator new[](sizeof(T) * 5 + x), and
		* new(2, f) T[5] results in a call of operator new[](sizeof(T) * 5 + y, 2, f).
		* x, y is overhead of array size
		*/

		//has_new<T>   T::operator new(size_t, ...)
		HAS_NEW_DELETE_DECL(operator new, new, void*, size_t);

		//has_array_new<T>   T::operator new[](size_t, ...)
		HAS_NEW_DELETE_DECL(operator new[], array_new, void*, size_t);
	}
}