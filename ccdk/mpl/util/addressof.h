#pragma once

#include<ccdk/mpl/mpl_module.h>

ccdk_namespace_mpl_util_start

#if !defined(CCDK_COMPILER_GCC)

	// return address of t
	template<class T>
	CCDK_FORCEINLINE constexpr T * addressof(T& t) noexcept {	
		return (__builtin_addressof(t));
	}
#else

	template<typename T>
	CCDK_FORCEINLINE constexpr T* addressof(T& t) noexcept
	{
		return reinterpret_cast<T*>(&const_cast<char&>(reinterpret_cast<const volatile char&>(t)));
	}
#endif

	template<class T>
	const T *addressof(const T&&) = delete;

ccdk_namespace_mpl_util_end