#pragma once

#include<typeinfo>
#include<exception>
#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/type_traits/decay.h>
#include<ccdk/mpl/util/forward.h>
#include<ccdk/mpl/util/swap.h>
#include<ccdk/mpl/util/addressof.h>
#include<ccdk/mpl/util/construct.h>
#include<ccdk/mpl/fusion/local_obj.h>

ccdk_namespace_mpl_fs_start

using namespace mpl;

template<typename T, uint32 N>
class local_arr
{
	constexpr uint32 kElementSize = sizeof(T);
	constexpr uint32 kTotalSize = kElementSize * N;
private:
	int8  content[kTotalSize];
public:
	/* default ctor */
	CCDK_FORCEINLINE constexpr local_arr() noexcept : content{} {}
	CCDK_FORCEINLINE constexpr local_arr(ptr::nullptr_t) noexcept : content{} {}

	// fill [0, n)
	template<typename... Args>
	CCDK_FORCEINLINE local_arr(ptr::size_t n, Args&& ... args) noexcept : content{} {
		util::construct_n<T>(content, n, util::forward<Args>(args)...);
	}

	// swap each element
	void swap(local_arr& other) {
		for (uint32 i = 0; i < N; ++i) {
			util::swap(address()[i], other[i]);
		}
	}

	CCDK_FORCEINLINE void operator=(local_arr const& other) noexcept {
		memcpy(content, other.construct, kTotalSize);
	}

	/* range fill */
	template<typename... Args>
	CCDK_FORCEINLINE void construct(ptr::size_t start, ptr::size_t n, Args&& ... args) {
		util::construct_n<T>( 
			reinterpret_cast<T*>(content) + start,
			n, util::forward<Args>(args)...);
	}

	/* range destruct */
	CCDK_FORCEINLINE void destruct(ptr::size_t start, ptr::size_t n) {
		util::destruct_n(reinterpret_cast<T*>(content) + start, n);
	}

	CCDK_FORCEINLINE T& operator[](uint32 idx) noexcept {
		return *( address() + idx);

	}
	CCDK_FORCEINLINE T const& operator[](uint32 idx) const noexcept {
		return *( address() + idx);
	}

	CCDK_FORCEINLINE T* address() noexcept {
		return reinterpret_cast<T*>(content);
	}

	CCDK_FORCEINLINE T const* address() const noexcept {
		return reinterpret_cast<T const*>(content);
	}

	CCDK_FORCEINLINE void rvalue_reset() {
		memset(content, 0, kTotalSize);
	}

};


ccdk_namespace_mpl_fs_end