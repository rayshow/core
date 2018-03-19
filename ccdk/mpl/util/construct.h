#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/base/logic_.h>
#include<ccdk/mpl/type_traits/is_pointer.h>
#include<ccdk/mpl/type_traits/has_deref.h>
#include<ccdk/mpl/type_traits/has_constructor.h>
#include<ccdk/mpl/iterator/iterator_traits.h>
#include<ccdk/mpl/util/destruct.h>
#include<ccdk/mpl/util/forward.h>
#include<ccdk/mpl/util/move.h>

ccdk_namespace_mpl_util_start

/* is proxy class(like iterator), try dereference It and get real address */
template<
	typename T,
	typename It,
	typename = check_t< not_< is_pointer<It>>>,
	typename = check_t< has_deref<It>>,
	typename... Args>
	CCDK_FORCEINLINE static void construct(It it, Args&& ... args)
{
	DebugFunctionName();
	new( util::addressof( *it ) ) T(util::forward<Args>(args)...);
}

/* truely pointer, directly construct on it */
template<typename T, typename... Args>
CCDK_FORCEINLINE static void construct(const void* memory, Args&& ... args)
{
	new( const_cast<void*>( memory)) T(util::forward<Args>(args)...);
}

namespace ut_impl
{

/////////////////////////////////////////////////////////////////////////////////////////
/////// construct copy range / n impl

	/* no-optim construct from iterator range */
	template<
		typename InputIt, typename ForwardIt,
		typename Source = iterator_value_t<InputIt>,
		typename Dest = iterator_value_t<ForwardIt>,
		typename = check_t< has_constructor<Dest,Source>>
	>
	ForwardIt construct_copy_range_impl(ForwardIt tbegin, InputIt fbegin, InputIt fend, opt_lv1)
	noexcept(has_nothrow_constructor_v<Dest, Source>)
	{
		DebugValue(" construct_copy_range  iterator copy");
		const InputIt it = fbegin;
		const ForwardIt it2 = tbegin;
		try { for (; it != fend; ++it, ++it2) construct< Dest >(it2, *it); }
		catch (...) { destruct_range(tbegin, it2); throw; }
		return it2;
	}

	/* trivial assign */
	template<
		typename InputIt, typename ForwardIt,
		typename Source = iterator_value_t<InputIt>,
		typename Dest = iterator_value_t<ForwardIt>,
		typename = check_t< has_assigner<Dest, Source>>
	>
	ForwardIt construct_copy_range_impl(ForwardIt tbegin, InputIt fbegin, InputIt fend, opt_lv2) noexcept {
		DebugValue(" construct_copy_range noexcept iterator copy");
		for (; fbegin != fend; ++fbegin, ++tbegin) *tbegin = *fbegin;
		return tbegin;
	}

	/* is byte pointer type, use memcpy-optimize */
	template< typename T1, typename T2 >
	CCDK_FORCEINLINE T1* construct_copy_range_impl(T1* tbegin, T2* fbegin, T2* fend, opt_lv3) noexcept  {
		DebugValue(" construct_copy_range memcpy copy");
		return static_cast<T1*>( memcpy(tbegin, fbegin, sizeof(T1)*(fend - fbegin)));
	}

	/* no-optim construct from iterator count range */
	template<
		typename InputIt, typename ForwardIt,
		typename Source = iterator_value_t<InputIt>,
		typename Dest = iterator_value_t<ForwardIt>,
		typename = check_t< has_constructor<Dest, Source>>
	>
	ForwardIt construct_copy_n_impl(ForwardIt tbegin, InputIt fbegin, ptr::size_t n, opt_lv1)
	{
		DebugValue(" construct_copy_n iterator copy");
		InputIt it= fbegin;
		ForwardIt it2 = tbegin;
		try { for (ptr::size_t c = 0; c<n; ++c, ++it, ++it2) construct< Dest >(it, *it2); }
		catch (...) { destruct_range(tbegin, it2); throw; }
		return it2;
	}

	/* trivial assign */
	template<
		typename InputIt, typename ForwardIt,
		typename Source = iterator_value_t<InputIt>,
		typename Dest = iterator_value_t<ForwardIt>,
		typename = check_t< has_assigner<Dest, Source>>
	>
	ForwardIt construct_copy_n_impl(ForwardIt tbegin, InputIt fbegin, ptr::size_t n, opt_lv2) noexcept {
		DebugValue(" construct_copy_n noexcept iterator copy");
		for (ptr::size_t c = 0; c < n; ++c, ++fbegin, ++tbegin) *tbegin = *fbegin;
		return tbegin;
	}

	/* is pod assign, use memcpy-optimize */
	template< typename T1, typename T2 >
	CCDK_FORCEINLINE T1* construct_copy_n_impl(T1* dest, T2* src, ptr::size_t n, opt_lv3) noexcept {
		DebugValue(" construct_copy_n memcpy copy");
		return static_cast<T1*>(memcpy(dest, src, sizeof(T1)*n));
	}

////////////////////////////////////////////////////////////////////////////////////////////////
///////  construct move range / n impl

	/* no-optim construct from iterator range */
	template<
		typename InputIt, typename ForwardIt,
		typename Source = iterator_value_t<InputIt>,
		typename Dest = iterator_value_t<ForwardIt>,
		typename = check_t< has_constructor<Dest, Source>>
	>
	ForwardIt construct_move_range_impl(ForwardIt tbegin, InputIt fbegin, InputIt fend, opt_lv1) noexcept
	{
		DebugValue(" construct_move_range  iterator copy");
		const InputIt it = fbegin;
		const ForwardIt it2 = tbegin;
		try { for (; it != fend; ++it, ++it2) construct< Dest >(it2, util::move(*it) ); }
		catch (...) { destruct_range(tbegin, it2); throw; }
		return it2;
	}

	template<
		typename InputIt, typename ForwardIt,
		typename Source = iterator_value_t<InputIt>,
		typename Dest = iterator_value_t<ForwardIt>,
		typename = check_t< has_assigner<Dest, Source>>
	>
	ForwardIt construct_move_range_impl(ForwardIt tbegin, InputIt fbegin, InputIt fend, opt_lv2) noexcept {
		DebugValue(" construct_move_range noexcept copy");
		for (; fbegin != fend; ++tbegin, ++fbegin) *tbegin = *fbegin;
		return tbegin;
	}

	/* is pod assign, use memcpy-optimize */
	template< typename T1, typename T2 >
	CCDK_FORCEINLINE T1* construct_move_range_impl(T1* tbegin, T2* fbegin, T2* fend, opt_lv3) noexcept {
		DebugValue(" construct_move_range memcpy copy");
		return static_cast<T1*>( memcpy(tbegin, fbegin, sizeof(T1)*(fend - fbegin)));
	}

	/* use move construct */
	template<
		typename InputIt, typename ForwardIt,
		typename Source = iterator_value_t<InputIt>,
		typename Dest = iterator_value_t<ForwardIt>,
		typename = check_t< has_constructor<Dest, Source >>
	>
	ForwardIt construct_move_n_impl(ForwardIt tbegin, InputIt fbegin, ptr::size_t n, opt_lv1) noexcept {
		DebugValue(" construct_move_n  it move");
		for (ptr::size_t c = 0; c<n; ++c, ++tbegin, ++fbegin) construct<Dest>(tbegin, util::move(*fbegin));
		return tbegin;
	}

	/* trivial copy */
	template<
		typename InputIt, typename ForwardIt,
		typename Source = iterator_value_t<InputIt>,
		typename Dest = iterator_value_t<ForwardIt>,
		typename = check_t< has_assigner<Dest, Source>>
	>
	ForwardIt construct_move_n_impl(ForwardIt tbegin, InputIt fbegin, ptr::size_t n, opt_lv2) noexcept {
		DebugValue(" construct_move_n noexcept it copy");
		for (ptr::size_t c = 0; c < n; ++c, ++fbegin, ++tbegin) *tbegin = *fbegin;
		return tbegin;
	}

	/* is trivial assign, use memcpy-optimize */
	template< typename T1, typename T2 >
	CCDK_FORCEINLINE T1* construct_move_n_impl(T1* dest, T2* src, ptr::size_t n, opt_lv3) noexcept
	{
		DebugValue(" construct_move_n memcpy copy");
		return static_cast<T1*>( memcpy(dest, src, sizeof(T1)*n) );
	}


///////////////////////////////////////////////////////////////////////////////////////////////////
/////  construct fill range / n impl 
///////////////////////////////////////////////////////////////////////////////////////////////////

	/* neither byte type nor trivial type, iterate construct fill */
	template<
		typename ForwardIt, typename T,
		typename Dest = iterator_value_t<ForwardIt>,
		typename = check_t< has_constructor<Dest,T>>
	>
	ForwardIt construct_fill_range_impl(ForwardIt begin, ForwardIt end, T const& t, opt_lv1)
		noexcept(has_nothrow_constructor_v<Dest, T>)
	{
		DebugValue(" uninitalized-range iterator fill");
		const ForwardIt it = begin;
		try { for (; it != end; ++it) construct< Dest >(it, t); }
		catch (...) { destruct_range(begin, it); throw; }
		return it;
	}

	/* rivial type, iterate assign fill */
	template< 
		typename ForwardIt, typename T, 
		typename Dest = iterator_value_t<ForwardIt>, 
		typename = check_t< has_assigner<Dest,T>>
	>
	ForwardIt construct_fill_range_impl(ForwardIt begin, ForwardIt end, T const& t, opt_lv2) noexcept {
		DebugValue(" uninitalized-range easy fill");
		for (; begin != end; ++begin) *begin = t; return begin;
	}


	/* is byte pointer type, use memset fill */
	template< typename It, typename T >
	CCDK_FORCEINLINE It* construct_fill_range_impl(It* begin, It* end, T const& t, opt_lv3) noexcept {
		DebugValue(" uninitalized-range memset fill");
		return memset(begin, t, end - begin);
	}

	/* not byte type, iterate fill */
	template<
		typename ForwardIt, typename T,
		typename Dest = iterator_value_t<ForwardIt>,
		typename = check_t< has_constructor<Dest, T>>
	>
	ForwardIt construct_fill_n_impl(ForwardIt begin, T const& t, ptr::size_t n, opt_lv1)
	{
		DebugValue(" uninitalized-n iterator fill");
		ForwardIt it = begin;
		try { for (ptr::size_t c = 0; c < n; ++c, ++it)  construct< Dest >(it, t); }
		catch (...) { destruct_range(begin, it); throw; }
		return it;
	}  

	/* not byte type, iterate fill */
	template<	
		typename ForwardIt, typename T,
		typename Dest = iterator_value_t<ForwardIt>,
		typename = check_t< has_assigner<Dest, T>>
	>
	ForwardIt construct_fill_n_impl(ForwardIt begin, T const& t, ptr::size_t n, opt_lv2) noexcept {
		DebugValue(" uninitalized-n no-except iterator fill");
		for (ptr::size_t c = 0; c < n; ++c, ++begin) *begin = t; return begin;
	}


	/* is byte pointer type, use memset fill */
	template< typename It, typename T >
	CCDK_FORCEINLINE It* construct_fill_n_impl(It* begin, T const& t, ptr::size_t n, opt_lv3) noexcept {
		DebugValue(" uninitalized-n memset fill");
		return static_cast<It*>( memset(begin, t, n));
	}


///////////////////////////////////////////////////////////////////////////////////////////////////////
/////// construct n impl

	/* only one arg, try use fill n */
	template< typename T, typename T2>
	CCDK_FORCEINLINE T* construct_n_impl(T* begin, ptr::size_t n, true_, T2 const& t)
		noexcept(has_nothrow_constructor_v<T,T2>)
	{
		return construct_fill_n_impl(begin, t, n, fill_opt_level_c<T*, T2>);
	}

	/* more then one arg, iterate over to construct  */
	template< typename T, typename... Args>
	CCDK_FORCEINLINE T* construct_n_impl(T* begin, ptr::size_t n, false_, Args&&... args)
		noexcept(has_nothrow_constructor_v<T, Args...>)
	{
		for (int i = 0; i < n; ++i, ++begin) { construct<T>(begin, util::forward<Args>(args)...); }
		return begin;
	}
}

/* construct iterator [tbegin, tbegin+fend-fbegin) with [fbegin, fend) */
template<
	typename InputIt, typename ForwardIt,
	typename Source = iterator_value_t<InputIt>,
	typename Dest = iterator_value_t<ForwardIt>,
	typename = check_t< has_constructor<Dest, Source>>
>
CCDK_FORCEINLINE ForwardIt construct_copy_range(ForwardIt tbegin, InputIt fbegin, InputIt fend)
	noexcept(has_constructor_v<Dest,Source>)
{
	return ut_impl::construct_copy_range_impl(tbegin, fbegin, fend, copy_opt_level_c<InputIt, ForwardIt>);
}

/* construct [tbegin, tbegin+n) from [fbegin, fbegin + n) */
template<
	typename InputIt, typename ForwardIt,
	typename Source = iterator_value_t<InputIt>,
	typename Dest = iterator_value_t<ForwardIt>,
	typename = check_t< has_constructor<Dest, Source>>
>
CCDK_FORCEINLINE ForwardIt construct_copy_n(ForwardIt tbegin, InputIt fbegin, ptr::size_t n)
noexcept(has_nothrow_constructor_v<Dest, Source>)
{
	if (n == 0) return tbegin;
	return ut_impl::construct_copy_n_impl(tbegin, fbegin, n, copy_opt_level_c<InputIt, ForwardIt>);
}


/* construct [tbegin, tend) with t */
template<
	typename ForwardIt, typename T,
	typename Dest = iterator_value_t<ForwardIt>,
	typename = check_t< has_constructor< Dest, T>>
>
CCDK_FORCEINLINE ForwardIt construct_fill_range(ForwardIt begin, ForwardIt end, T const& t)
noexcept(has_nothrow_constructor_v<Dest, T>)
{
	DebugValue(" construct_fill_range:", has_nothrow_constructor_v<Dest, T>);
	return ut_impl::construct_fill_range_impl(begin, end, t, fill_opt_level_c<ForwardIt, T>);
}

/* construct [tbegin, tbegin+n) with t */
template<
	typename ForwardIt, typename T,
	typename Dest = iterator_value_t<ForwardIt>,
	typename = check_t< has_constructor<Dest, T>>
>
CCDK_FORCEINLINE ForwardIt construct_fill_n(ForwardIt begin, T const& t, ptr::size_t n )
	noexcept(has_nothrow_constructor_v<Dest, T>)
{
	if (n == 0) return begin;
	return ut_impl::construct_fill_n_impl(begin, t, n, fill_opt_level_c<ForwardIt, T>);
}


/* construct [tbegin, tbegin + fend-fbegin) moving from [fbegin, fend) */
template<
	typename InputIt, typename ForwardIt,
	typename Source = iterator_value_t<InputIt>,
	typename Dest = iterator_value_t<ForwardIt>,
	typename = check_t< has_constructor<Dest, Source>>
>
CCDK_FORCEINLINE ForwardIt construct_move_range(ForwardIt tbegin, InputIt fbegin, InputIt fend) noexcept
{
	return ut_impl::construct_move_range_impl(tbegin, fbegin, fend, copy_opt_level_c<InputIt, ForwardIt>);
}

/* copy [begin, begin+n) to [begin2, begin2 + n) */
template<
	typename InputIt, typename ForwardIt,
	typename Source = iterator_value_t<InputIt>,
	typename Dest = iterator_value_t<ForwardIt>,
	typename = check_t< has_constructor<Dest, Source>>
>
CCDK_FORCEINLINE ForwardIt construct_move_n(ForwardIt tbegin, InputIt fbegin, ptr::size_t n) noexcept
{
	if (n == 0) return tbegin;
	return ut_impl::construct_move_n_impl(tbegin, fbegin, n, copy_opt_level_c<InputIt, ForwardIt>);
}

/* inplace construct in [ (T*)memory, (T*)memory+n) with args... */
template<typename T, typename... Args>
CCDK_FORCEINLINE static T* construct_n(void* memory, ptr::size_t n, Args&& ... args)
{
	if (n == 0) return static_cast<T*>(memory);
	return ut_impl::construct_n_impl(static_cast<remove_cv_t<T>*>(memory), n,
		bool_c<sizeof...(Args) == 1>, util::forward<Args>(args)...);
}


ccdk_namespace_mpl_util_end