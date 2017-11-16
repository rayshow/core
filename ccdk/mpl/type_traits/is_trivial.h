#pragma once

#include<ccdk/mpl/base/bool_.h>

namespace ccdk
{
	namespace mpl
	{
		// Is Trivial
		// is a scaler ,array(T is trivial) or a class(included struct union)
		//   with trivial copyable and default trivial constructor,may be cv-qualifed
		// charactoristic:
		// 1. contiguous storage for trivial copy ==>can memcpy to another
		//    same type obj and easy seralization and deseralization from char but not compatible with pod( 
		//    bacause allow different level of access,the compiler can arrange member freely
		//    is this contigous storage)
		// 2. static default initalization for trivial default constructor

		// type can be opt with:
		// without constructor can use malloc alloc memory , free memory no need destructor
		// memset / memcpy  can be use with trivial type


		// trivial constructor: (member mean non-static member)
		// 1. no user-defined constructor (except defined constructor with default)
		// 2. no virtual member(include derive from base class)
		// 3. base and member is trivial constructor
		// 4. member with no direct = and {} initalizers
		// trivial copyable 
		// copy constructor and copy assignment is trivial
		template<typename T> struct is_trivial :public bool_<__is_trivial(T)> {};
		template<typename T> constexpr bool is_trivial_v = is_trivial<T>::value;
	}
}