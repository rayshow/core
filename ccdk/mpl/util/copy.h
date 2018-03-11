#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/base/logic_.h>
#include<ccdk/mpl/base/arithmetic_.h>
#include<ccdk/mpl/type_traits/has_assigner.h>
#include<ccdk/mpl/util/addressof.h>
#include<ccdk/mpl/iterator/iterator_traits.h>
#include<cstring>

ccdk_namespace_mpl_util_start

/* 2-value copy assign */
template<
	typename T, typename T2,
	typename = check_t< has_assigner<T, T2>> >
CCDK_FORCEINLINE void copy(T& dest, const T2& src) 
	noexcept( has_nothrow_assigner_v<T, T2> )
{
	dest = src; 
}

/* trivial-type array copy*/
template<
	typename T, , typename T2,
	ptr::size_t D,  ptr::size_t S,
	typename = check_t< can_do_memcpy<T,T2>> >
CCDK_FORCEINLINE void copy(T(&dest)[D], const T2(&src)[S]) noexcept
{
	static constexpr ptr::size_t Min = min_val<ptr::size_t, D, S>;
	memcpy((void*)addressof(dest), (void*)addressof(src), sizeof(T)*Min);
}

/* non-trivial-type array copy*/
template<
	typename T, typename T2,
	ptr::size_t D, ptr::size_t S,
	typename = check_t< not_<can_do_memcpy<T,T2>>>,  /* T is not trivial  */
	typename = check_t< has_copy_assigner<T>> >      /* and has copy asssigner */
CCDK_FORCEINLINE void copy(T(&dest)[D], const T2(&src)[S])
	noexcept(has_nothrow_assigner_v<T, T2>)
{
	constexpr ptr::size_t Min = min_val<ptr::size_t, D, S>;
	for (int i = 0; i < Min; ++i) { dest[i] = src[i]; }
}


namespace ut_impl
{
	template<
		typename It, typename It2,
		typename Dest = iterator_value_t<It>,
		typename Source = iterator_value_t<It2>
	>
	CCDK_FORCEINLINE It copy_range_impl(It tbegin, It2 fbegin, It2 fend, false_)
		noexcept(has_nothrow_assigner_v<Dest, Source>)
	{
		for (; fbegin != fend; ++fbegin, ++tbegin) *tbegin = *fbegin;
		return tbegin;
	}

	template<typename It, typename It2>
	CCDK_FORCEINLINE It* copy_range_impl(It* tbegin, It2* fbegin, It2* fend, true_) noexcept {
		return memcpy(tbegin, fbegin, fend - fbegin); 
	}

	template<
		typename It, typename It2,
		typename Dest = iterator_value_t<It>,
		typename Source = iterator_value_t<It2>
	>
	CCDK_FORCEINLINE It copy_n_impl(It dest, It2 src, ptr::size_t n, false_)
		noexcept(has_nothrow_assigner_v<Dest, Source>)
	{
		for (ptr::size_t i = 0; i < n; ++i, ++dest, ++src) *dest = src;
		return dest;
	}

	template<typename It, typename It2>
	CCDK_FORCEINLINE It* copy_n_impl(It* dest, It2* src, ptr::size_t n, true_) noexcept {
		return memcpy(dest, src, n); 
	}
}


/* copy [fbegin, fend) to [tbegin, tbegin + fend-fbegin) */
template<
	typename It, typename It2,
	typename Dest = iterator_value_t<It>,  
	typename Source = iterator_value_t<It2>, 
	typename = check_t< has_assigner<Dest,Source>> >
CCDK_FORCEINLINE void copy_range(It tbegin, It2 fbegin, It2 fend)
	noexcept( has_nothrow_assigner_v<Dest,Source>)
{
	return ut_impl::copy_range_impl(tbegin, fbegin, fend, can_do_memcpy_c<It, It2>);
}


template< 
	typename It,
	typename It2,
	typename Source = iterator_value_t<It>,
	typename Dest = iterator_value_t<It2>,
	typename = check_t< has_assigner<Dest, Source>>>
CCDK_FORCEINLINE void copy_n(It dest, It2 src, ptr::size_t n)
	noexcept(has_nothrow_assigner_v<Dest, Source>)
{
	return ut_impl::copy_n_impl(dest, src, n, can_do_memcpy_c<It, It2>);
}

ccdk_namespace_mpl_util_end
