#pragma once

#include<ccdk/compile.h>

namespace ccdk
{
	namespace mpl
	{
		namespace util
		{
			template<class T>
			CCDK_FORCEINLINE constexpr 
			T *addressof(T& _Val) noexcept
			{	// return address of _Val
				return (__builtin_addressof(_Val));
			}

			template<class _Ty>
			const _Ty *addressof(const _Ty&&) = delete;
		}
	}
}