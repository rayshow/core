#pragma once

namespace core
{
	namespace mpl
	{
		template<typename T> struct remove_ref      { typedef T type; };
		template<typename T> struct remove_ref<T&>  { typedef T type; };
		template<typename T> struct remove_ref<T&&> { typedef T type; };
		template<typename T> using remove_ref_t = typename remove_ref<T>::type;

		template<typename T> struct remove_rref { typedef T type; };
		template<typename T> struct remove_rref<T&&> { typedef T type; };
		template<typename T> using remove_rref_t = typename remove_rref<T>::type;
	}
}
