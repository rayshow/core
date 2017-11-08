#pragma once

#include<core/preprocessor/seq_compose.h>
#include<core/preprocessor/seq_foreach_item.h>
#include<core/preprocessor/sign.h>
#include<core/mpl/bool_.h>


namespace Aurora3D
{
	namespace mpl
	{
		template<typename T> struct IsMemberFnPtr :public False_ {};
#define IS_MEMBER_FUNCTION(Postfix,prefix,...)                                                                                              \
		template<typename C, typename Ret, typenameprefix,... Args> struct IsMemberFnPtr<Ret (C::*)(Argsprefix,...) Postfix> :public True_ {};     \
		template<typename C, typename Ret, typenameprefix,... Args> struct IsMemberFnPtr<Ret (C::*)(Argsprefix,...,prefix,...) Postfix> :public True_ {};
		A3D_PP_FOREACH_ITEM(IS_MEMBER_FUNCTION, A3D_PP_COMPOSE_EX((const, volatile, const volatile), (&, &&, A3D_PP_NULL), A3D_PP_NULL, &, &&))
#undef IS_FUNCTION

#define IsMemberFnPtrV(T)   (IsMemberFnPtr<T>::value)
#define NotMemberFnPtrV(T)  (!IsMemberFnPtr<T>::value)
	}
}