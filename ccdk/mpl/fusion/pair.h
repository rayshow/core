#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/base/type_.h>
#include<ccdk/mpl/base/enable_if.h>
#include<ccdk/mpl/type_traits/is_empty.h>
#include<ccdk/mpl/type_traits/is_final.h>
#include<ccdk/mpl/type_traits/has_constructor.h>
#include<ccdk/mpl/mcontainer/arg_pack.h>
#include<ccdk/mpl/util/forward.h>
#include<ccdk/mpl/util/swap.h>

ccdk_namespace_mpl_fs_start

/* index value pair */
template<uint32 Key, typename T>
struct ipair
{
	typedef decay_t<T> value_type;
	template<uint32, typename> friend struct ipair;

	value_type value;

	CCDK_FORCEINLINE constexpr ipair() :value{} {  }

	/* container need check T2 convertible to T , allow narrow cast */
	template<typename T2, typename = check_t< has_constructor<value_type, T2>>>
	CCDK_FORCEINLINE explicit constexpr ipair(T2&& t) : value( util::forward<T2>(t) )  {}

	/* copy */
	CCDK_FORCEINLINE constexpr ipair(ipair const& other) : value{ other.value } {  }

	/* move */
	CCDK_FORCEINLINE constexpr ipair(ipair&& other) : value{ util::move(other.value) } {}

	/* template copy */
	template<uint32 Key2, typename T2,  typename = check_t< has_constructor<value_type, T2> > >
	CCDK_FORCEINLINE constexpr ipair(ipair<Key2, T2> const& other) : value{ other.value } {  }

	/* template move */
	template<uint32 Key2, typename T2, typename = check_t< has_constructor<value_type, T2> > >
	CCDK_FORCEINLINE constexpr ipair(ipair<Key2, T2> && other) : value{ util::move(other.value) } {  }

	template<uint32 Key2, typename T2, typename = check_t< has_constructor<value_type, T2> > >
	CCDK_FORCEINLINE void swap(ipair<Key2, T2>& other) { using namespace util; swap(value, other.value); }

};


/* get value at Key */
template<uint32 Key, typename T> CCDK_FORCEINLINE constexpr decay_t<T> & ipair_at(ipair<Key, T> & pi) noexcept { return pi.value; }
template<uint32 Key, typename T> CCDK_FORCEINLINE constexpr decay_t<T> const& ipair_at(ipair<Key, T> const& pi) noexcept { return pi.value; }

/* adl swap of ipair */
template<uint32 Key1, uint32 Key2, typename T1, typename T2, typename = check_t< is_compatible<T1, T2>> >
CCDK_FORCEINLINE void swap(ipair<Key1, T1>& lh, ipair<Key2, T2>& rh) noexcept { lh.swap(rh); }

/* equal of ipair */
template<uint32 Key1, uint32 Key2, typename T1, typename T2, typename = check_t< is_compatible<T1, T2>> >
CCDK_FORCEINLINE bool operator==(ipair<Key1, T1>& lh, ipair<Key2, T2>& rh) noexcept { return lh.value == rh.value; }

/* less of ipair */
template<uint32 Key1, uint32 Key2, typename T1, typename T2, typename = check_t< is_compatible<T1, T2>> >
CCDK_FORCEINLINE bool operator<(ipair<Key1, T1>& lh, ipair<Key2, T2>& rh) noexcept { return lh.value < rh.value; }


/* index reference pair */
template<uint32 Key, typename T>
struct irpair
{
	T& value;

	CCDK_FORCEINLINE constexpr irpair(T&& u) : value{ u } { }
};


template<uint32 Key, typename T> CCDK_FORCEINLINE constexpr T& irpair_at(irpair<Key, T> & pi) noexcept {	return pi.value; }
template<uint32 Key, typename T> CCDK_FORCEINLINE constexpr T const& irpair_at(irpair<Key, T> const& pi) noexcept { return pi.value; }


/* compress pair */
template<typename First, typename Second, bool = is_empty_v<First> && !is_final_v<First> >
struct cpair: public First
{
	typedef decay_t<Second> second_type;
	template<typename, typename, bool> friend struct cpair;
private:
	second_type scd;

public:
	/* deefault */
	CCDK_FORCEINLINE constexpr cpair() : First{}, scd{} {}

	/* value constructor */
	template<typename U, typename = check_t< has_constructor<second_type, U>>>
	CCDK_FORCEINLINE explicit constexpr cpair(U&& u) : First(), scd(util::forward<U>(u)) {}

	template<typename T, typename U, typename = check_t< has_constructor<First, T>>, typename = check_t< has_constructor<second_type, U>> >
	CCDK_FORCEINLINE constexpr cpair(T&& t, U&& u) : First(util::forward<T>(t)), scd(util::forward<U>(u)) {}

	/* copy */
	CCDK_FORCEINLINE constexpr cpair(cpair const& other) : First{ other }, scd{ other.scd } {}

	/* move */
	CCDK_FORCEINLINE constexpr cpair(cpair && other) : First{ util::move(other) }, scd{ util::move(other.scd) } {}

	/* template copy, for compatible type */
	template<typename T, typename U, typename = check_t< has_constructor<First, T>>, typename = check_t< has_constructor<second_type, U>> >
	CCDK_FORCEINLINE constexpr cpair(cpair<T,U> const& other) : First{ other.first() }, scd{ other.second() } {}

	/* template move, for compatible type */
	template<typename T, typename U, typename = check_t< has_constructor<First, T>>, typename = check_t< has_constructor<second_type, U>> >
	CCDK_FORCEINLINE constexpr cpair(cpair<T, U> && other) : First{ util::move(other.first()) }, scd{ util::move(other.second()) } {}

	template<typename T, typename U, typename = check_t< has_constructor<First, T>>, typename = check_t< has_constructor<second_type, U>> >
	CCDK_FORCEINLINE void swap(cpair<T, U>& other) noexcept {  util::swap(scd, other.scd); }

	CCDK_FORCEINLINE constexpr First& first() noexcept { return *this; }
	CCDK_FORCEINLINE constexpr const First& first() const noexcept { return *this; }

	CCDK_FORCEINLINE constexpr second_type& second() noexcept { return scd; }
	CCDK_FORCEINLINE constexpr const second_type& second() const noexcept { return scd; }

	
};

template<typename First, typename Second>
struct cpair<First,Second,false>
{
	typedef decay_t<First> first_type;
	typedef decay_t<Second> second_type;
	template<typename, typename, bool> friend struct cpair;
private:
	first_type  fst;
	second_type scd;

public:

	template<typename U>
	CCDK_FORCEINLINE constexpr cpair(U&& u) : fst{}, scd(util::forward<U>(u)) {}

	template<typename T, typename U, typename = check_t< has_constructor<first_type, T>>, typename = check_t< has_constructor<second_type, U>> >
	CCDK_FORCEINLINE constexpr cpair(T&& t, U&& u) : fst(util::forward<T>(t)), scd(util::forward<U>(u)) {}

	/* copy */
	CCDK_FORCEINLINE constexpr cpair(cpair const& other) : fst{ other.fst }, scd{ other.scd } {}

	/* move */
	CCDK_FORCEINLINE constexpr cpair(cpair && other) : fst{ util::move(other.fst) }, scd{ util::move(other.scd) } {}

	/* template copy, for compatible type */
	template<typename T, typename U, typename = check_t< has_constructor<first_type, T>>, typename = check_t< has_constructor<second_type, U>> >
	CCDK_FORCEINLINE constexpr cpair(cpair<T, U> const& other) : fst{ other.first() }, scd{ other.second() } {}

	/* template move, for compatible type */
	template<typename T, typename U, typename = check_t< has_constructor<first_type, T>>, typename = check_t< has_constructor<second_type, U>> >
	CCDK_FORCEINLINE constexpr cpair(cpair<T, U> && other) : fst{ util::move(other.first()) }, scd{ util::move(other.second()) } {}

	template<typename T, typename U, typename = check_t< has_constructor<first_type, T>>, typename = check_t< has_constructor<second_type, U>> >
	CCDK_FORCEINLINE void swap(cpair<T, U>& other) noexcept { using namespace util; swap(fst, other.fst); swap(scd, other.scd); }

	CCDK_FORCEINLINE constexpr First& first() noexcept { return fst; }
	CCDK_FORCEINLINE constexpr const First& first() const noexcept { return fst; }

	CCDK_FORCEINLINE constexpr second_type& second() noexcept { return scd; }
	CCDK_FORCEINLINE constexpr const second_type& second() const noexcept { return scd; }
};

/* get value at Key type */
template<typename Key, typename T> CCDK_FORCEINLINE constexpr decay_t<T> & cpair_at(cpair<Key, T> & pi) noexcept { return pi.second(); }
template<typename Key, typename T> CCDK_FORCEINLINE constexpr decay_t<T> const& cpair_at(cpair<Key, T> const& pi) noexcept { return pi.second(); }

template<typename T1, typename T2, typename U1, typename U2, typename = check_t< is_compatible<T1, T2>>, typename = check_t< is_compatible<U1, U2>> >
CCDK_FORCEINLINE void swap(cpair<T1, U1>& lh, cpair<T2, U2>& rh) noexcept { lh.swap(rh); }

template<typename T1, typename T2, typename U1, typename U2, typename = check_t< is_compatible<T1, T2>>, typename = check_t< is_compatible<U1, U2>> >
CCDK_FORCEINLINE bool operator==(cpair<T1, U1>& lh, cpair<T2, U2>& rh) noexcept { lh.first() == rh.first() && lh.second() == rh.second(); }


/* normal pair */
template<typename First, typename Second>
struct pair
{
	First  first;
	Second second;
};

/* make normal pair */
template<typename T1, typename T2>
auto make_pair(T1&& t1, T2&& t2) { return pair<T1, T2>{t1, t2}; }


ccdk_namespace_mpl_fs_end