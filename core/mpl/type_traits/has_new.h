#pragma once

#include<new>
#include<core/mpl/bool_.h>
#include<core/mpl/type_traits/has_operator_type.h>
#include<core/mpl/type_traits/has_new_delete_decl.h>

namespace Aurora3D
{
	namespace mpl
	{
		//HasDefaultNew<T>   T::operator new(size_t)
		HAS_NEW_DELETE_DECL(operator new, DefaultNew, size_t);

		//HasNothrowNew<T>   T::operator new(size_t, const std::nothrow_t&)
		HAS_NEW_DELETE_DECL(operator new, NothrowNew, size_t, const std::nothrow_t&);

		//HasPlacementNew<T> T::operator new(size_t, void*)
		HAS_NEW_DELETE_DECL(operator new, PlacementNew, size_t, void*);

		//HasNothrowPlacementNew<T>  T::operator new(size_t, const std::nothrow_t&, void*)
		HAS_NEW_DELETE_DECL(operator new, NothrowPlacementNew, size_t, const std::nothrow_t&, void*);

		//HasDerfaultArrayNew<T>   T::operator new[](size_t)
		HAS_NEW_DELETE_DECL(operator new[], DefaultArrayNew, size_t);

		//HasNothrowArrayNew<T>   T::operator new[](size_t, const std::nothrow_t&)
		HAS_NEW_DELETE_DECL(operator new[], NothrowArrayNew, size_t, const std::nothrow_t&);

		//HasPlacementArrayNew<T> T::operator new[](size_t, void*)
		HAS_NEW_DELETE_DECL(operator new[], PlacementArrayNew, size_t, void*);

		//HasNothrowPlacementNew<T>  T::operator new[](size_t, const std::nothrow_t&, void*)
		HAS_NEW_DELETE_DECL(operator new[], NothrowPlacementArrayNew, size_t, const std::nothrow_t&, void*);

	}
}