#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/base/type_.h>
#include<ccdk/mpl/base/if_.h>
#include<ccdk/mpl/base/arithmetic_.h>
#include<ccdk/mpl/units/units_fwd.h>

ccdk_namespace_mpl_units_start

namespace detail
{
	constexpr int64 gcd(int64 v1, int64 v2)
	{
		int64 left = 0;
		while (v2)
		{
			left = v1 % v2;
			v1 = v2;
			v2 = left;
		}
		return v1;
	}

	constexpr int64 lcm(int64 v1, int64 v2)
	{
		return v1 * v2 / gcd(v1, v2);
	}

	constexpr int64 abs(int64 v)
	{
		return (((v >> 62) & 2) - 1) * -v;
	}
}

template< int64 N, int64 D = 1LL >
struct ratio
{
	static_assert(D != 0, "den is 0");

	static constexpr int64 gcd = detail::gcd(detail::abs(N), detail::abs(D));
	static constexpr int64 num = N / gcd;
	static constexpr int64 den = D / gcd;

	static constexpr double value = double(N) / double(D);

	template<typename T>
	static constexpr T multiply(T const& t) { return value * double(t); }

	template< typename Other>
	struct add
	{
		typedef ratio< num * Other::den + den * Other::num, den * Other::den> type;
	};

	template< typename Other>
	struct sub
	{
		typedef ratio< num * Other::den - den * Other::num, den * Other::den> type;
	};

	template<typename Other>
	struct mul
	{
		typedef ratio< num * Other::num, den * Other::num> type;
	};

	template<typename Other>
	struct div
	{
		typedef ratio< num * Other::den, den * Other::num> type;
	};

	template<typename Other>
	struct less: bool_< (num * Other::den < Other::num * den) > {};

	template<typename Other>
	struct equal : bool_ < (num * Other::den == Other::num * den) >{};
};


/* convert value from ratio<N1,D1> to ratio<N2,N2> */
template<int64 N1, int64 D1, int64 N2, int64 D2>
struct converter_< ratio<N1, D1>, ratio<N2, D2> >
{
	static constexpr bool value = true;       

	typedef typename div_< ratio<N1, D1>, ratio<N2, D2>>::type type;
	static constexpr double factor = (double)type::num / (double)type::den;

	template<typename T>
	constexpr static T apply(T const& t) { return t*factor; }
};

/* choose small denominator ratio as main transform target */
template< int64 N1, int64 D1, int64 N2, int64 D2>
struct main_transform< ratio<N1, D1>, ratio<N2, D2> >
	: if_c< (D1<D2), ratio<N1, D1>, ratio<N2, D2> >{};

// scalar
typedef ratio<1, 1000000000000000000LL> atto;
typedef ratio<1, 1000000000000000LL> femto;
typedef ratio<1, 1000000000000LL> pico;
typedef ratio<1, 1000000000> nano;
typedef ratio<1, 1000000> micro;
typedef ratio<1, 1000> milli;
typedef ratio<1, 100> centi;
typedef ratio<1, 10> deci;
typedef ratio<1, 1> uniform;
typedef ratio<10, 1> deca;
typedef ratio<100, 1> hecto;
typedef ratio<1000, 1> kilo;
typedef ratio<1000000, 1> mega;
typedef ratio<1000000000, 1> giga;
typedef ratio<1000000000000LL, 1> tera;
typedef ratio<1000000000000000LL, 1> peta;
typedef ratio<1000000000000000000LL, 1> exa;

namespace storage
{
	typedef ratio<1, 8LL>  bit;
	typedef ratio<1>  byte;
	typedef ratio<2>  word;
	typedef ratio<2>  dword;
	typedef ratio<1024LL> kb;
	typedef ratio<1024LL * 1024LL> mb;
	typedef ratio<1024LL * 1024LL * 1024LL> gb;
	typedef ratio<1024LL * 1024LL * 1024LL * 1024> tb;
	typedef ratio<1024LL * 1024LL * 1024LL * 1024LL * 1024LL> pb;
	typedef ratio<1024LL * 1024LL * 1024LL * 1024LL * 1024LL * 1024LL> eb;
}

ccdk_namespace_mpl_units_end