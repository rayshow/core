
#pragma once
#include<core/mpl/logic_not.h>
#include<core/mpl/logic_and.h>
#include<core/mpl/type_traits/is_member_ptr.h>
#include<core/mpl/type_traits/is_member_fn_ptr.h>

namespace Aurora3D
{
	namespace mpl
	{
		template<typename T> struct IsMemberObjPtr :public And<IsMemberPtr<T>, Not<IsMemberFnPtr<T>> > {};
		
#define IsMemberObjPtrV(T)   (IsMemberObjPtr<T>::value)
#define NotMemberObjPtrV(T)  (!IsMemberObjPtr<T>::value)
	}
}