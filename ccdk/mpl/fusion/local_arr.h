#pragma once

#include<typeinfo>
#include<exception>
#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/type_traits/decay.h>
#include<ccdk/mpl/util/forward.h>
#include<ccdk/mpl/util/construct.h>
#include<ccdk/mpl/util/swap.h>
#include<ccdk/mpl/util/addressof.h>
#include<ccdk/mpl/util/construct.h>
#include<ccdk/mpl/fusion/local_obj.h>
#include<ccdk/mpl/iterator/iterator_traits.h>
#include<ccdk/mpl/iterator/algorithm/distance.h>

ccdk_namespace_mpl_fs_start

using namespace mpl;

template<typename T, uint32 N>
class local_arr
{
public:
	static constexpr uint32 kElementSize = sizeof(T);
	static constexpr uint32 kByteSize = kElementSize * N;
	using size_type = uint32;
private:
	int8   content[kByteSize];
public:
	/* default ctor */
	CCDK_FORCEINLINE constexpr local_arr() noexcept : content{} {}
	CCDK_FORCEINLINE constexpr local_arr(ptr::nullptr_t) noexcept : content{} {}

	// fill [0, n)
	template<typename... Args>
	CCDK_FORCEINLINE local_arr(size_type n, Args&& ... args) noexcept : content{} {
		util::construct_n<T>(content, n, util::forward<Args>(args)...);
	}

	// copy-n
	template<typename InputIt, typename = check_t< is_iterator<InputIt>>>
	CCDK_FORCEINLINE local_arr(InputIt beginIt, size_type n) {
		util::construct_copy_n(address(), beginIt, n);
	}

	// copy-range
	template<typename InputIt, typename = check_t< is_iterator<InputIt>>>
	CCDK_FORCEINLINE local_arr(InputIt beginIt, InputIt endIt) {
		util::construct_copy_n(address(), beginIt, it::distance(beginIt,endIt));
	}

	// move n 
	CCDK_FORCEINLINE local_arr(local_arr const& other, size_type n) {
		util::construct_move_n(address(), other.address(), n);
	}

	// tempalte move n 
	template<uint32 N2>
	CCDK_FORCEINLINE local_arr(local_arr<T,N2> const& other, size_type n) {
		util::construct_move_n(address(), other.address(), fn::min(N,n) );
	}

	// swap each element
	template<uint32 Size>
	void swap_n(local_arr& other) {
		static constexpr uint32 kSwapSize = Size * sizeof(T);
		uint8 tmp[kSwapSize];
		memcpy(tmp, other.content, kSwapSize);
		memcpy(other.content, content, kSwapSize);
		memcpy(content, tmp, kSwapSize);
	}

	CCDK_FORCEINLINE void operator=(local_arr const& other) noexcept {
		memcpy(content, other.construct, kTotalSize);
	}

	CCDK_FORCEINLINE void operator=(local_arr && other) noexcept {
		memcpy(content, other.construct, kTotalSize);
	}

	// range-fill
	template<typename... Args>
	CCDK_FORCEINLINE void construct(ptr::size_t start, ptr::size_t n, Args&& ... args) {
		util::construct_n<T>(address() + start, n, util::forward<Args>(args)...);
	}

	// range-copy
	template<typename InputIt>
	CCDK_FORCEINLINE void construct(ptr::size_t start, ptr::size_t n, InputIt beginIt) {
		util::construct_copy_n( address() + start, beginIt, n);
	}

	// range de-ctor
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