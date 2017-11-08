#pragma once
namespace Aurora3D
{
	namespace mpl
	{

#define ValueV(T)        (T::value)
#define ValueT(T)        (typename T::value_type)

		template<int64 N> struct Int_;
		template<bool B> struct Bool_;

		template<typename T, T N>
		struct Integral_
		{
			static constexpr T value = N;
			typedef T value_type;
			constexpr operator T() const
			{
				return N;
			}
		};
	}
}