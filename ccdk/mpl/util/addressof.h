#pragma once

#include<ccdk/compile.h>

namespace ccdk
{
	namespace mpl
	{
		namespace util
		{
#if !defined(CCDK_COMPILER_GCC)
			template<class T>
			CCDK_FORCEINLINE constexpr 
			T *addressof(T& t) noexcept
			{	// return address of _Val
				return (__builtin_addressof(t));
			}
#else

			template<typename T>
			CCDK_FORCEINLINE constexpr
			T* addressof(T& t) noexcept
			{
				return reinterpret_cast<T*>
					(&const_cast<char&>(reinterpret_cast<const volatile char&>(t)));
			}
#endif

			template<class _Ty>
			const _Ty *addressof(const _Ty&&) = delete;
		}
	}
}