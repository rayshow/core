#pragma once

#include<core/mpl/base/bool_.h>
#include<core/mpl/base/size_.h>
#include<core/mpl/type_traits/impl/has_operator_type.h>
#include<core/mpl/type_traits/impl/has_new_delete_decl.h>

namespace std
{
	struct nothrow_t;


}

namespace core
{
	namespace mpl
	{
		
		//HasDefaultNew<T>   T::operator new(size_t)
		HAS_NEW_DELETE_DECL(operator new, default_new, size_t);

		//HasNothrowNew<T>   T::operator new(size_t, const std::nothrow_t&)
		HAS_NEW_DELETE_DECL(operator new, nothrow_new, size_t, const std::nothrow_t&);

		//HasPlacementNew<T> T::operator new(size_t, void*)
		HAS_NEW_DELETE_DECL(operator new, placement_new, size_t, void*);

		//HasNothrowPlacementNew<T>  T::operator new(size_t, const std::nothrow_t&, void*)
		HAS_NEW_DELETE_DECL(operator new, nothrow_place_new, size_t, const std::nothrow_t&, void*);

		//HasDerfaultArrayNew<T>   T::operator new[](size_t)
		HAS_NEW_DELETE_DECL(operator new[], default_array_new, size_t);

		//HasNothrowArrayNew<T>   T::operator new[](size_t, const std::nothrow_t&)
		HAS_NEW_DELETE_DECL(operator new[], nothrow_array_new, size_t, const std::nothrow_t&);

		//HasPlacementArrayNew<T> T::operator new[](size_t, void*)
		HAS_NEW_DELETE_DECL(operator new[], placement_array_new, size_t, void*);

		//HasNothrowPlacementNew<T>  T::operator new[](size_t, const std::nothrow_t&, void*)
		HAS_NEW_DELETE_DECL(operator new[], nothrow_placement_array_new, size_t, const std::nothrow_t&, void*);

	}
}