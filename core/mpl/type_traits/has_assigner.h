#pragma once

#include<core/mpl/base/bool_.h>
#include<core/mpl/base/or_.h>
#include<core/mpl/type_traits/remove_cv.h>
#include<core/mpl/type_traits/is_base_same.h>
#include<core/mpl/type_traits/add_const_lref.h>
#include<core/mpl/type_traits/add_lref.h>
namespace core
{
	namespace mpl
	{
		// L && =? R && 
#define HAS_ASSIGNER(L, R)           __is_assignable(L, R)
#define HAS_TRIVIAL_ASSIGNER(L, R)   __is_trivially_assignable(L, R)
#define HAS_NOTHROW_ASSIGNER(L, R)   __is_nothrow_assignable(L, R)

		//string() = 1  right value assign,  for  compound type, works because compound inner member data is always left value
		//int() = 1     literal assign ,  for  fundumental type, always false, not work 
		template<typename L, typename R> struct has_rassigner:public bool_<HAS_ASSIGNER(L, R)>{};
		template<typename L, typename R> struct has_trivial_rassigner :public bool_<HAS_TRIVIAL_ASSIGNER(L, R)> {};
		template<typename L, typename R> struct has_nothrow_rassigner :public bool_<HAS_NOTHROW_ASSIGNER(L, R)> {};
		
		// use assign function to judge
		//L t = R&&  left value assign
		//const L = R&&  don't work???????? , use IsBaseSame workaround
		//Base* = Derive*  works
		//Base =  Derive   works
		template<typename L, typename R> struct has_assigner : or< bool_<HAS_ASSIGNER( add_lref_t<L>, R) >, is_base_same<L,R>> {};
		template<typename L, typename R> struct has_trivial_assigner :public or < bool_<HAS_TRIVIAL_ASSIGNER(AddLValueRefT<L>, R)>, IsBaseSame<L, R>> {};
		template<typename L, typename R> struct has_nothrow_assigner :public or < bool_<HAS_NOTHROW_ASSIGNER(AddLValueRefT<L>, R)>, IsBaseSame<L, R>> {};

		//T a = const T& b
		template<typename T> struct HasCopyAssigner :public bool_<HAS_ASSIGNER( AddLValueRefT<T>,  ConvertConstLRefT<T> )> {};
		template<typename T> struct HasTrivialCopyAssigner :public bool_<HAS_TRIVIAL_ASSIGNER(AddLValueRefT<T>,  ConvertConstLRefT<T>)> {};
		template<typename T> struct HasNoThrowCopyAssigner :public bool_<HAS_NOTHROW_ASSIGNER(AddLValueRefT<T>,  ConvertConstLRefT<T>)> {};

		//T a = T&& b
		template<typename T> struct HasMoveAssigner :public bool_<HAS_ASSIGNER(AddLValueRefT<T>, T)> {};
		template<typename T> struct HasTriviallyMoveAssigner :public bool_<HAS_TRIVIAL_ASSIGNER(AddLValueRefT<T>, T)> {};
		template<typename T> struct HasNoThrowMoveAssigner :public bool_<HAS_NOTHROW_ASSIGNER(AddLValueRefT<T>, T)> {};

#undef HAS_ASSIGNER
#undef HAS_TRIVIAL_ASSIGNER
#undef HAS_NOTHROW_ASSIGNER
	}
}