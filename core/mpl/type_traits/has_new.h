#pragma once

#include<core/mpl/base/bool_.h>
#include<core/mpl/base/size_.h>
#include<core/mpl/type_traits/impl/has_operator_type.h>
#include<core/mpl/type_traits/impl/has_new_delete_decl.h>

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

		//defeat
		// 1. temp class define in function can't check, always false
		// 2. parameter type must be same, int is diff from int& or float

		//has_new<T>   T::operator new(size_t, ...)
		HAS_NEW_DELETE_DECL(operator new, new, void*, size_t);

		//has_array_new<T>   T::operator new[](size_t, ...)
		HAS_NEW_DELETE_DECL(operator new[], array_new, void*, size_t);
	}
}