#pragma once

#include<core/mpl/base/bool_.h>
#include<core/mpl/type_traits/impl/has_operator_type.h>
#include<core/mpl/type_traits/impl/has_new_delete_decl.h>

namespace core
{
	namespace mpl
	{
		/*
		* delete ptr results in a call of operator delete(ptr),
		* delete(2) ptr results in a call of operator delete(ptr, 2),
		*/

		//defeat 
		// 1. temp class define in function can't check, always false
		// 2. parameter type must be same, int is diff from int& or float

		//has_default_delete<T>   T::operator delete(void* memory,...)
		HAS_NEW_DELETE_DECL(operator delete, delete, void, void*);

		//has_default_array_delete<T>  T::operator delete[](void*,...)
		HAS_NEW_DELETE_DECL(operator delete[], array_delete, void, void*);
	}
}