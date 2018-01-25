#pragma once

namespace ccdk {
	namespace mpl {
		//T * const ==> const (T*)  bottom-level const, pointer can't change
		//const T * ==> (const T)*  top-level const, content can't change
		//T & const ==> const (T&) ==> T&  lvalue-reference can't change to another obj, so const is redundant
		//const T & ==> (const T)&  top-level const, content can't change
		//const T[N] ==>const (T[2]) first get const,  is bottom const
		//   consist of const T array

		//T    => T  const
		//T*   => T* const
		//void => const void
		//T&   => T&  const => T&
		//T&&  => T&& const => T&&
		//(T[n]) const <=> T const [n] 
		//add bottom-level const 
		template<typename T> struct add_const { typedef T const type; };
		template<typename T> using add_const_t = typename add_const<T>::type;
	}
}
