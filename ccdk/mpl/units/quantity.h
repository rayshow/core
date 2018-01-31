#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/base/bool_.h>
#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/type_traits/compatible_type.h>
#include<ccdk/mpl/type_traits/is_compatible.h>
#include<ccdk/mpl/type_traits/has_constructor.h>
#include<ccdk/mpl/mcontainer/add.h>
#include<ccdk/mpl/mcontainer/mul_.h>
#include<ccdk/mpl/util/move.h>
#include<ccdk/mpl/util/forward.h>
#include<ccdk/mpl/units/dimension.h>
#include<ccdk/mpl/units/ratio.h>
 
ccdk_namespace_mpl_units_start

#define quantity_new_type_decl()                                                        \
typedef typename main_transform< Transform, Transform2>::type       transform_type2;    \
typedef typename compatible_type<ValueType, ValueType2>::type       value_type2;        \
typedef typename quantity< value_type2, Dimension, transform_type2> new_quantity;       \
typedef typename quantity< ValueType2, Dimension, Transform2>       other_quantity;


template<typename ValueType, typename Dimension, typename Transform>
class quantity
{
public:
	typedef quantity   this_type;
	typedef ValueType  value_type;
	template<typename,typename,typename>
	friend class quantity;

private:
	value_type value;
public:
	/* default initalized */
	CCDK_FORCEINLINE constexpr quantity() :value{} {}

	/* value initalized */
	template<typename T, typename = check_t< has_constructor<value_type, T>>>
	CCDK_FORCEINLINE constexpr explicit quantity(T&& t) :value( util::forward<T>(t) ) {}

	/* same type copy */
	CCDK_FORCEINLINE constexpr quantity(const quantity& other) : value{ other.value } {}

	/*template copy */
	template<
		typename ValueType2, typename Transform2, 
		typename = check_t< has_constructor<ValueType,ValueType2>>,   /* ValueType2 convert to ValueType is availible */
		typename = check_t< has_converter<Transform2,Transform>>      /* Transform2 to Trnasform is availible, e.g. inch to cm */
	>
	CCDK_FORCEINLINE constexpr quantity(const quantity<ValueType2, Dimension, Transform2>& other)
		: value{ converter_<Transform2,Transform>::template apply(other.value) }
	{}

	/* add two quantity, ValueType and Trnasform may be different */
	template<
		typename ValueType2, typename Transform2,
		typename = check_t< has_constructor<ValueType, ValueType2>>,   /* ValueType2 convert to ValueType is availible */
		typename = check_t< has_converter<Transform2, Transform>>      /* Transform2 to Transform is availible, e.g. inch to cm */
	>
	auto operator+(const quantity<ValueType2, Dimension, Transform2>& other)
	{
		quantity_new_type_decl();
		return copy_or_ref_t<this_type, new_quantity>{ *this } + copy_or_ref_t < other_quantity, new_quantity>{ other };
	}

	/* add two quantity, ValueType and Trnasform may be different */
	template<
		typename ValueType2, typename Transform2,
		typename = check_t< has_constructor<ValueType, ValueType2>>,   /* ValueType2 convert to ValueType is availible */
		typename = check_t< has_converter<Transform2, Transform>>      /* Transform2 to Transform is availible, e.g. inch to cm */
	>
		auto operator-(const quantity<ValueType2, Dimension, Transform2>& other)
	{
		quantity_new_type_decl();
		return copy_or_ref_t<this_type, new_quantity>{ *this } - copy_or_ref_t < other_quantity, new_quantity>{ other };
	}

	/* multiply different quantity */
	template<
		typename ValueType2, typename Dimension2, typename Transform2,
		typename = check_t< is_compatible<ValueType, ValueType2>>,     /* ValueType2 convert to ValueType is availible */
		typename = check_t< has_converter<Transform2, Transform>>      /* Transform2 to Transform is availible, e.g. inch to cm */
	>
		auto operator*(const quantity<ValueType2, Dimension2, Transform2>& other)
	{
		typedef typename mul_< Transform, Transform2>::type                         transform_type2;    
		typedef typename compatible_type<ValueType, ValueType2>::type               value_type2;
		typedef typename add_< Dimension, Dimension2>::type                         dimension_type2;
		return quantity< value_type2, dimension_type2, transform_type2>{ other.value * value };
	}

	/* divide different quantity */
	template<
		typename ValueType2, typename Dimension2, typename Transform2,
		typename = check_t< has_constructor<ValueType, ValueType2>>,   /* ValueType2 convert to ValueType is availible */
		typename = check_t< has_converter<Transform2, Transform>>      /* Transform2 to Transform is availible, e.g. inch to cm */
	>
		auto operator/(const quantity<ValueType2, Dimension2, Transform2>& other)
	{
		typedef typename mul_< Transform, Transform2>::type                         transform_type2;
		typedef typename compatible_type<ValueType, ValueType2>::type               value_type2;
		typedef typename add_< Dimension, Dimension2>::type                         dimension_type2;
		return quantity< value_type2, dimension_type2, transform_type2>{ other.value * value };
	}

	operator value_type() const noexcept
	{
		return value;
	}
};

#undef quantity_new_type_decl

/* length quantity */
template<typename Transform>
using length =  quantity<default_value_type, length_, Transform>; /* different kind transform of length  */
using ulength = length< uniform >;          /* default base length is 1m, all other length depends on it */

/* meter */
template<char... args>
constexpr auto operator""_m() noexcept { return ulength{  ccdk_literial_parse_float(args) }; }

/* kilometer */
template<char... args>
constexpr auto operator""_km() noexcept{ return length< kilo >{ ccdk_literial_parse_float(args) }; }


ccdk_namespace_mpl_units_end