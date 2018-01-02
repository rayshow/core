#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/util/create.h>
#include<ccdk/mpl/fusion/pair.h>

ccdk_namespace_mpl_fs_start

/* index reference map */
template< typename Indice, typename... Args >
class irmap;

template<uint32 ... Indice, typename ... Args>
class irmap< indice_pack<Indice...>, Args...>
	: irpair< Indice, Args >...
{
public:
	CCDK_FORCEINLINE constexpr irmap(Args &&... args) : irpair<Indice, Args>{ util::forward<Args>(args) }... {}

	template<uint32 Key>
	CCDK_FORCEINLINE constexpr decltype(auto) at() noexcept { return irpair_at<Key>(*this); }

	template<uint32 Key>
	CCDK_FORCEINLINE constexpr decltype(auto) at() const noexcept { return irpair_at<Key>(*this); }
};

template< typename Indice, typename... Args >
class imap;

/* index map */
template<uint32 ... Indice, typename... Args>
class imap< indice_pack<Indice...>, Args...>
		: ipair < Indice, Args >...
{
public:

	/*default*/
	CCDK_FORCEINLINE constexpr imap() : ipair < Indice, Args >{}... {}

	/*value*/
	template<typename... Args2, typename = check_t< and_< is_convertible<Args2, decay_t<Args>>...>>>
	CCDK_FORCEINLINE constexpr imap(Args2 &&... args) : ipair<Indice, Args>{ util::forward<Args2>(args) }... {  }

	/*copy*/
	CCDK_FORCEINLINE constexpr imap(imap const& other) : ipair < Indice, Args >{ other }... {}

	/*move*/
	CCDK_FORCEINLINE constexpr imap(imap && other) : ipair < Indice, Args >{ util::move(other) }... {}

	/*template copy*/
	template<typename... Args2, typename = check_t< and_< is_convertible<Args2, decay_t<Args>>...>>>
	CCDK_FORCEINLINE constexpr imap(imap<indice_pack<Indice...>, Args2...> const& other) : ipair < Indice, Args >{ other.template at<Indice>() }... {}

	/*template move*/
	template<typename... Args2, typename = check_t< and_< is_convertible<Args2, decay_t<Args>>...>>>
	CCDK_FORCEINLINE constexpr imap(imap<indice_pack<Indice...>, Args2...> && other) : ipair < Indice, Args >{ util::move(other.template at<Indice>()) }... {}

	template<uint32 Key>
	CCDK_FORCEINLINE constexpr decltype(auto) at() noexcept { return ipair_at<Key>(*this); }

	template<uint32 Key>
	CCDK_FORCEINLINE constexpr decltype(auto) at() const noexcept { return ipair_at<Key>(*this); }
};

/* adl swap of imap */
template<typename Indice, typename... Args1, typename... Args2, typename = check_t< and_< is_compatible<Args2, Args2>...>>>
CCDK_FORCEINLINE void swap(imap<Indice, Args1...>& lh, imap<Indice,Args2...>& rh) noexcept
{
	imap<Indice, Args1...> tmp{ util::move(lh) };
	lh = util::move(rh);
	rh = util::move(tmp);
}

template<typename... Args>
using reference_args = irmap< make_indice< sizeof...(Args)>, Args...>;

template<uint32 Size, typename... Args>
using closure_args = imap< make_indice< Size >, Args...>;

constexpr util::create_t< reference_args > create_reference_args{};

ccdk_namespace_mpl_fs_end