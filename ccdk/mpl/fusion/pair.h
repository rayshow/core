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

// common object fn of access pair/cpair/rpair
struct get_first_t {
	template<typename Pair>
	CCDK_FORCEINLINE decltype(auto) operator()(Pair const& p) const noexcept {
		return p.first();
	}
	template<typename Pair>
	CCDK_FORCEINLINE decltype(auto) operator()(Pair & p) const noexcept {
		return p.first();
	}
};

struct get_second_t {
	template<typename Pair>
	CCDK_FORCEINLINE decltype(auto) operator()(Pair const& p) const noexcept {
		return p.second();
	}
	template<typename Pair>
	CCDK_FORCEINLINE decltype(auto) operator()(Pair & p) const noexcept {
		return p.second();
	}
};


////////////////////////////////////////////////////////////////////////////
//// normal pair
template<typename First, typename Second>
struct pair
{
	First  _1;
	Second _2;

	CCDK_FORCEINLINE First& first() noexcept { return _1; }
	CCDK_FORCEINLINE First const& first() const noexcept { return _1; }

	CCDK_FORCEINLINE Second& second() noexcept { return _2; }
	CCDK_FORCEINLINE Second const& second() const noexcept { return _2; }
};

// make normal pair 
template<typename T1, typename T2>
auto make_pair(T1&& t1, T2&& t2) { return pair<remove_ref_t<T1>, remove_ref_t<T2>>{t1, t2}; }

/////////////////////////////////////////////////////////////////////////////////////
//// reference pair
template<typename First, typename Second>
struct rpair
{
	First&  _1;
	Second& _2;

	// tie(p1,p2) = pair<T1,T2>
	template<typename T1, typename T2>
	CCDK_FORCEINLINE void operator=(pair<T1, T2> const& p) {
		_1 = p._1;
		_2 = p._2;
	}

	// tie(p1,p2) = util::move( pair<T1,T2> )
	template<typename T1, typename T2>
	CCDK_FORCEINLINE void operator= (pair<T1, T2>&& p) {
		_1 = util::move(p._1);
		_2 = util::move(p._2);
	}

	//access first
	CCDK_FORCEINLINE First& first() noexcept { return _1; }
	CCDK_FORCEINLINE First const& first() const noexcept { return _2; }

	//access second
	CCDK_FORCEINLINE Second& second() noexcept { return _1; }
	CCDK_FORCEINLINE Second const& second() const noexcept { return _2; }

};

// make rpair
template<typename T1, typename T2>
CCDK_FORCEINLINE rpair<T1, T2> tie(T1& t1, T2& t2) { return { t1, t2 }; }

/////////////////////////////////////////////////////////////////////////////////////
//// compress pair
template<typename First, typename Second, bool = is_empty_v<First> && !is_final_v<First> >
struct cpair : public First
{
	typedef decay_t<Second> second_type;
	template<typename, typename, bool> friend struct cpair;
private:
	second_type _2;

public:
	// default
	CCDK_FORCEINLINE constexpr cpair() = default;

	// constructor from second compatible type, first default construct
	template<typename U, typename = check_t< has_constructor<second_type, U>>>
	CCDK_FORCEINLINE explicit constexpr cpair(U&& u) : First(), _2(util::forward<U>(u)) {}

	//constructor from two compatible type
	template<typename T, typename U,
		typename = check_t< has_constructor<First, T>>,
		typename = check_t< has_constructor<second_type, U>> >
	CCDK_FORCEINLINE constexpr cpair(T&& t, U&& u) 
		: First(util::forward<T>(t)), _2(util::forward<U>(u)) {}

	// copy
	CCDK_FORCEINLINE constexpr cpair(cpair const& other) 
		: First{ other.first() }, _2{ other._2 } {}

	// move
	CCDK_FORCEINLINE constexpr cpair(cpair && other) 
		: First{ util::move(other.first()) }, _2{ util::move(other._2) } {}

	// template copy, for compatible type 
	template<typename T, typename U,
		typename = check_t< has_constructor<First, T>>,
		typename = check_t< has_constructor<second_type, U>> >
	CCDK_FORCEINLINE constexpr cpair(cpair<T, U> const& other) 
		: First{ other.first() }, _2{ other._2 } {}

	// template move, for compatible type
	template<typename T, typename U,
		typename = check_t< has_constructor<First, T>>,
		typename = check_t< has_constructor<second_type, U>> >
	CCDK_FORCEINLINE constexpr cpair(cpair<T, U> && other) 
		: First{ util::move(other.first()) }, _2{ util::move(other._2) } {}

	//template swap
	template<typename T, typename U,
		typename = check_t< has_constructor<First, T>>,
		typename = check_t< has_constructor<second_type, U>> >
	CCDK_FORCEINLINE void swap(cpair<T, U>& other) noexcept { util::swap(_2, other._2); }

	//access first
	CCDK_FORCEINLINE constexpr First& first() noexcept { return *this; }
	CCDK_FORCEINLINE constexpr const First& first() const noexcept { return *this; }

	//access second
	CCDK_FORCEINLINE constexpr second_type& second() noexcept { return _2; }
	CCDK_FORCEINLINE constexpr const second_type& second() const noexcept { return _2; }


};

template<typename First, typename Second>
struct cpair<First, Second, false>
{
	typedef decay_t<First> first_type;
	typedef decay_t<Second> second_type;
	template<typename, typename, bool> friend struct cpair;
private:
	first_type  _1;
	second_type _2;

public:

	//default 
	CCDK_FORCEINLINE constexpr cpair() = default;

	// constructor from second compatible type, first default construct
	template<typename U>
	CCDK_FORCEINLINE constexpr cpair(U&& u) : _1{}, _2(util::forward<U>(u)) {}

	//constructor from two compatible type
	template<typename T, typename U,
		typename = check_t< has_constructor<first_type, T>>,
		typename = check_t< has_constructor<second_type, U>> >
	CCDK_FORCEINLINE constexpr cpair(T&& t, U&& u) 
		: _1(util::forward<T>(t)), _2(util::forward<U>(u)) {}

	// copy
	CCDK_FORCEINLINE constexpr cpair(cpair const& other) : _1{ other._1 }, _2{ other._2 } {}

	//move
	CCDK_FORCEINLINE constexpr cpair(cpair && other) : _1{ util::move(other._1) }, _2{ util::move(other._2) } {}

	//template copy
	template<typename T, typename U, typename = check_t< has_constructor<first_type, T>>, typename = check_t< has_constructor<second_type, U>> >
	CCDK_FORCEINLINE constexpr cpair(cpair<T, U> const& other) : _1{ other.first() }, _2{ other.second() } {}

	//template move
	template<typename T, typename U, typename = check_t< has_constructor<first_type, T>>, typename = check_t< has_constructor<second_type, U>> >
	CCDK_FORCEINLINE constexpr cpair(cpair<T, U> && other) : _1{ util::move(other.first()) }, _2{ util::move(other.second()) } {}

	//template swap
	template<typename T, typename U,
		typename = check_t< has_constructor<first_type, T>>,
		typename = check_t< has_constructor<second_type, U>> >
	CCDK_FORCEINLINE void swap(cpair<T, U>& other) noexcept {
		util::swap(_1, other._1); 
		util::swap(_2, other._2); 
	}

	//access first
	CCDK_FORCEINLINE constexpr First& first() noexcept { return _1; }
	CCDK_FORCEINLINE constexpr const First& first() const noexcept { return _1; }

	//access second
	CCDK_FORCEINLINE constexpr second_type& second() noexcept { return _2; }
	CCDK_FORCEINLINE constexpr const second_type& second() const noexcept { return _2; }
};

/* get value at Key type */
template<typename Key, typename T> CCDK_FORCEINLINE constexpr decay_t<T> & cpair_at(cpair<Key, T> & pi) noexcept { return pi.second(); }
template<typename Key, typename T> CCDK_FORCEINLINE constexpr decay_t<T> const& cpair_at(cpair<Key, T> const& pi) noexcept { return pi.second(); }

template<typename T1, typename T2, typename U1, typename U2, typename = check_t< is_compatible<T1, T2>>, typename = check_t< is_compatible<U1, U2>> >
CCDK_FORCEINLINE void swap(cpair<T1, U1>& lh, cpair<T2, U2>& rh) noexcept { lh.swap(rh); }

template<typename T1, typename T2, typename U1, typename U2, typename = check_t< is_compatible<T1, T2>>, typename = check_t< is_compatible<U1, U2>> >
CCDK_FORCEINLINE bool operator==(cpair<T1, U1>& lh, cpair<T2, U2>& rh) noexcept { lh.first() == rh.first() && lh.second() == rh.second(); }

/////////////////////////////////////////////////////////////////////////////////////
//// compile-time index pair
template<uint32 Key, typename T>
struct ipair
{
	typedef decay_t<T> value_type;
	template<uint32, typename> friend struct ipair;

	value_type value;

	//default
	CCDK_FORCEINLINE ipair() = default;

	// second use compatible type
	template<typename T2, 
		typename = check_t< has_constructor<value_type, T2>>>
	CCDK_FORCEINLINE explicit constexpr ipair(T2&& t) 
		: value( util::forward<T2>(t) )  {}

	//copy
	CCDK_FORCEINLINE constexpr ipair(ipair const& other) 
		: value{ other.value } {  }

	//move
	CCDK_FORCEINLINE constexpr ipair(ipair&& other) 
		: value{ util::move(other.value) } {}

	//template copy
	template<
		uint32 Key2, typename T2,
		typename = check_t< has_constructor<value_type, T2> > >
	CCDK_FORCEINLINE constexpr ipair(ipair<Key2, T2> const& other) 
		: value{ other.value } {  }

	// template move
	template<
		uint32 Key2, typename T2,
		typename = check_t< has_constructor<value_type, T2> > >
	CCDK_FORCEINLINE constexpr ipair(ipair<Key2, T2> && other) 
		: value{ util::move(other.value) } { }


	//template swap
	template<
		uint32 Key2, typename T2,
		typename = check_t< has_constructor<value_type, T2> > >
	CCDK_FORCEINLINE void swap(ipair<Key2, T2>& other) { 
		util::swap(value, other.value); 
	}
};

// compile-time get second value by index
template<uint32 Key, typename T> CCDK_FORCEINLINE constexpr decay_t<T>& ipair_at(ipair<Key, T> & pi) noexcept { return pi.value; }
template<uint32 Key, typename T> CCDK_FORCEINLINE constexpr decay_t<T> const& ipair_at(ipair<Key, T> const& pi) noexcept { return pi.value; }

// equal of ipair
template<uint32 Key1, uint32 Key2, typename T1, typename T2, typename = check_t< is_compatible<T1, T2>> >
CCDK_FORCEINLINE bool operator==(ipair<Key1, T1>& lh, ipair<Key2, T2>& rh) noexcept { return lh.value == rh.value; }

// less of ipair
template<
	uint32 Key1, uint32 Key2,
	typename T1, typename T2, 
	typename = check_t< is_compatible<T1, T2>> >
CCDK_FORCEINLINE bool operator<(ipair<Key1, T1>& lh, ipair<Key2, T2>& rh) noexcept {
	return lh.value < rh.value; 
}


/////////////////////////////////////////////////////////////////////////////////////
//// compile-time index reference pair
template<uint32 Key, typename T>
struct irpair
{
	T& value;

	CCDK_FORCEINLINE constexpr irpair() = default;
	CCDK_FORCEINLINE constexpr irpair(T&& u) : value{ u } { }
};

// compile-time get second value by index
template<uint32 Key, typename T> CCDK_FORCEINLINE constexpr T& irpair_at(irpair<Key, T> & pi) noexcept {	return pi.value; }
template<uint32 Key, typename T> CCDK_FORCEINLINE constexpr T const& irpair_at(irpair<Key, T> const& pi) noexcept { return pi.value; }




ccdk_namespace_mpl_fs_end