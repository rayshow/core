#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/base/bool_.h>
#include<ccdk/mpl/base/compile_check.h>
#include<ccdk/mpl/util/move.h>
#include<ccdk/mpl/type_traits/has_constructor.h>
#include<ccdk/mpl/units/dimension.h>
#include<ccdk/mpl/units/ratio.h>

ccdk_namespace_mpl_units_start


template<typename ValueType, typename Dimension, typename Transform>
class quantity
{
public:
	typedef quantity   this_type;
	typedef ValueType  value_type;
private:
	value_type value;
public:
	/* default initalized */
	CCDK_FORCEINLINE constexpr quantity() :value{} {}

	/* value initalized */
	CCDK_FORCEINLINE constexpr explicit quantity(const value_type& inValue) :value{ inValue } {}

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

	template<
		typename ValueType2, typename Transform2,
		typename = check_t< has_constructor<ValueType, ValueType2>>,   /* ValueType2 convert to ValueType is availible */
		typename = check_t< has_converter<Transform2, Transform>>      /* Transform2 to Trnasform is availible, e.g. inch to cm */
	>
	auto operator+(const quantity<ValueType2, Dimension, Transform2>& other)
	{

	}
};



/* length define, default standard base units is 1m */
template<typename Transform>
using length =  quantity<default_value_type, length_, Transform>;
using ulength = length< uniform>;

/* */
template<typename Transform>
using 

ccdk_namespace_mpl_units_end