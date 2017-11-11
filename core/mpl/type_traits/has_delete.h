#pragma once

#include<new>
#include<core/mpl/base/bool_.h>
#include<core/mpl/type_traits/impl/has_operator_type.h>
#include<core/mpl/type_traits/impl/has_new_delete_decl.h>

namespace core
{
	namespace mpl
	{
		//has_default_delete<T>   T::operator delete(void* memory)
		HAS_NEW_DELETE_DECL(operator delete, default_delete, void*);

		//has_virtual_delete<T> T::operator delete(void* memory, size_t size)
		//for Base pointer delete Derived Class
		HAS_NEW_DELETE_DECL(operator delete, virtual_delete, void*, size_t);

		//has_default_array_delete<T>  T::operator delete[](void*)
		HAS_NEW_DELETE_DECL(operator delete[], default_array_delete, size_t);

		//has_virtual_array_delete<T>   T::operator delete[](void, size_t size)
		//for Base pointer delete Derived Class
		HAS_NEW_DELETE_DECL(operator delete[], virtual_array_delete, void*, size_t);
	}
}