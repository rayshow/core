#pragma once

#include<ccdk/mpl/base/bool_.h>
#include<ccdk/mpl/base/and_.h>
#include<ccdk/mpl/base/not_.h>
#include<ccdk/mpl/type_traits/is_rref.h>
#include<ccdk/mpl/type_traits/add_const_lref.h>
#include<ccdk/mpl/type_traits/add_lref.h>
#include<ccdk/mpl/type_traits/declval.h>

namespace ccdk
{
	namespace mpl
	{
		namespace detail
		{
			template<typename T, typename P>
			struct has_assigner_helper
			{
				template<typename U, typename = decltype(makeval<P>() = declval<U>())>
				static constexpr bool sfinae(int) { return true; }
				template<typename U> static constexpr bool sfinae(...) { return false; }
				static constexpr bool value = sfinae<T>(0);
			};
		}

		//although lvalue assign like std::string() = "xx"; is valid, but still forbidden here for useless of this style
		template<typename T, typename F> struct has_assigner : and_< not_<is_rref<T>>, detail::has_assigner_helper<F, T>> {};
		template<typename T> struct has_copy_assigner :public has_assigner< add_lref_t<T>, add_const_lref_t<T> > {};
		template<typename T> struct has_move_assigner :public has_assigner< add_lref_t<T>, add_rref_t<T> > {};

		template<typename T, typename F> struct has_nothrow_assigner : and_< not_<is_rref<T>>, has_assigner<T,F>, bool_< noexcept(makeval<T>() = declval<F>())>> {};
		template<typename T> struct has_nothrow_copy_assigner :public has_nothrow_assigner< add_lref_t<T>, add_const_lref_t<T> > {};
		template<typename T> struct has_nothrow_move_assigner :public has_nothrow_assigner< add_lref_t<T>, add_rref_t<T> > {};

		template<typename T, typename F> struct has_trivial_assigner : and_< not_<is_rref<T>>, bool_<__is_trivially_assignable(F,T)>>{};
		template<typename T> struct has_trivial_copy_assigner :public has_trivial_assigner<add_lref_t<T>, add_const_lref_t<T>> {};
		template<typename T> struct has_trivial_move_assigner :public has_trivial_assigner< add_lref_t<T>, add_rref<T>> {};

		template<typename T, typename F> static constexpr bool has_assigner_v = has_assigner<T, F>::value;
		template<typename T> static constexpr bool has_copy_assigner_v = has_copy_assigner<T>::value;
		template<typename T> static constexpr bool has_move_assigner_v = has_move_assigner<T>::value;

		template<typename T, typename F> static constexpr bool has_nothrow_assigner_v = has_nothrow_assigner<T,F>::value;
		template<typename T> static constexpr bool has_nothrow_copy_assigner_v = has_nothrow_copy_assigner<T>::value;
		template<typename T> static constexpr bool has_nothrow_move_assigner_v = has_nothrow_move_assigner<T>::value;

		template<typename T, typename F> static constexpr bool has_trivial_assigner_v = has_trivial_assigner<T,F>::value;
		template<typename T> static constexpr bool has_trivial_copy_assigner_v = has_trivial_copy_assigner<T>::value;
		template<typename T> static constexpr bool has_trivial_move_assigner_v = has_trivial_move_assigner<T>::value;
	}
}