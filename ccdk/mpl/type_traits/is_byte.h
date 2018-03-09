#pragma once

#include<ccdk/mpl/base/type_.h>
namespace ccdk
{
	namespace mpl
	{
		template<typename T> struct is_byte :false_ {};
		template<> struct is_byte<char> :true_ {};
		template<> struct is_byte<uint8>:true_ {};
		template<> struct is_byte<int8> :true_ {};
		template<> struct is_byte<bool> :true_ {};
	}
}