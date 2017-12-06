#pragma once
#include<ccdk/type.h>

namespace ccdk
{
	namespace mpl
	{
		struct normal_deleter
		{
			template<typename T>
			constexpr void operator()(T*& t)
			{
				ptr::safe_delete(t);
			}
		};

		struct normal_derive_deleter
		{
			template<typename T>
			constexpr void __delete(T*& t)
			{
				ptr::safe_delete(t);
			}
		};

		struct normal_array_deleter
		{
			template<typename T>
			constexpr void operator()(T*& t)
			{
				ptr::safe_delete_array(t);
			}
		};

		struct normal_derive_array_deleter
		{
			template<typename T>
			constexpr void __delete(T*& t)
			{
				ptr::safe_delete_array(t);
			}
		};
	}
}