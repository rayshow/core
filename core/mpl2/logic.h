#pragma once
#include"bool.h"
#include"if.h"

namespace Aurora3D
{
	namespace mpl
	{
		//lazy or,and,not
		template<typenameprefix,... Args>
		struct Or
		{
			static constexpr bool value = false;
		};

		template<typename First, typenameprefix,... Other>
		struct Or<First, Otherprefix,...> :
			public TypeIf<First, True_, Or<Otherprefix,...>>::type
		{};


		template<typenameprefix,... Args>
		struct And
		{
			static constexpr bool value = true;
		};

		template<typename First, typenameprefix,... Other>
		struct And<First, Otherprefix,...> :
			public TypeIf<First, And<Otherprefix,...>, False_>::type
		{};


		template<typename T>
		struct Not : public Bool_< !T::value > {};
	}
}