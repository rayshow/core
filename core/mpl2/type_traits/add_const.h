#pragma once

namespace Aurora3D
{
	namespace mpl 
	{
		//kownledge point:
		//T * const ==> const (T*)  bottom-level const, first get const 
		//const T * ==> (const T)*  top-level const, first get pointer
		//T const * ==> (T const)*  top-level const, first get pointer
		//T & const ==> const (T&) ==> T&  lvalue-reference can't change content, so const is redundant
		//const T & ==> (const T)&  refer to a const T value is meaningful
		//T const & ==> (T const)&  equal to const int & style
		//const T&& ==> (const T)&& right refer to a unchangible value is't meaningful
		//const T[N] ==>const (T[2]) first get const,  is bottom const
		//T const [N] ==> const (T[2]) first get const

		//(const T) *   content can't change,but pointer can change
		//const (T[2])  content can't change, T[N] as a new type
		//(const T) &   content can't change


		//T    => T  const
		//T*   => T* const
		//void => const void
		//T&   => T&  const => T&
		//T&&  => T&& const => T&&

		//add bottom-level const
		template<typename T> struct AddConst { typedef T const type; };
		template<typename T> using AddConstT = typename AddConst<T>::type;

	}
}

