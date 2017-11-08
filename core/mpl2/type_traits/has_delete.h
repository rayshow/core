#pragma once

#include<new>
#include<core/mpl/bool_.h>
#include<core/mpl/type_traits/has_operator_type.h>
#include<core/mpl/type_traits/has_new_delete_decl.h>

namespace Aurora3D
{
	namespace mpl
	{
		//HasDefaultDelete<T>   T::operator delete(void* memory)
		HAS_NEW_DELETE_DECL(operator delete, DefaultDelete, void*);

		//HasVirtualDelete<T> T::operator delete(void* memory, size_t size)
		//for Base pointer delete Derived Class
		HAS_NEW_DELETE_DECL(operator delete, VirtualDelete, void*, size_t);

		//HasDefaultArrayDelete<T>  T::operator delete[](void*)
		HAS_NEW_DELETE_DECL(operator delete[], DefaultArrayDelete, size_t);

		//HasVirtualArrayDelete<T>   T::operator delete[](void, size_t size)
		//for Base pointer delete Derived Class
		HAS_NEW_DELETE_DECL(operator delete[], VirtualArrayDelete, void*, size_t);
	}
}