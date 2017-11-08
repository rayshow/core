#pragma once

#include<core/mpl/bool_.h>

namespace Aurora3D
{
	namespace mpl
	{
		//can modification  accord to platform
#define HAS_DESTRUCTOR(T)           __is_destructible(T)
#define HAS_TRIVIAL_DESTRUCTOR(T)   __has_trivial_destructor(T)
#define HAS_NOTHROW_DESTRUCTOR(T)   __is_nothrow_destructible(T)
#define HAS_VIRTUAL_DESTRUCTOR(T)   __has_virtual_destructor(T)

		template<typename T> struct HasDestructor:public Bool_<HAS_DESTRUCTOR(T)> {};
		template<typename T> struct HasVirtualDestructor :public Bool_<HAS_VIRTUAL_DESTRUCTOR(T)> {};
		template<typename T> struct HasTrivialDestructor :public Bool_<HAS_TRIVIAL_DESTRUCTOR(T)> {};
		template<typename T> struct HasNoThrowDestructor :public Bool_<HAS_NOTHROW_DESTRUCTOR(T)> {};

#undef HAS_DESTRUCTOR
#undef HAS_TRIVIAL_DESTRUCTOR
#undef HAS_NOTHROW_DESTRUCTOR
#undef HAS_VIRTUAL_DESTRUCTOR
	}
}