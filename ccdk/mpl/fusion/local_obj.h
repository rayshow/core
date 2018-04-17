#pragma once

#include<typeinfo>
#include<exception>
#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/type_traits/decay.h>
#include<ccdk/mpl/util/forward.h>
#include<ccdk/mpl/util/swap.h>
#include<ccdk/mpl/util/addressof.h>
#include<ccdk/mpl/util/construct.h>
#include<ccdk/mpl/fusion/bad_cast_exception.h>

ccdk_namespace_mpl_fs_start

template<typename T>
struct local_obj
{
private:
	int8  bytes[sizeof(T)];

public:
	//default ctor
	CCDK_FORCEINLINE constexpr local_obj() noexcept = default;

	// implace ctor
	template<typename... Args>
	CCDK_FORCEINLINE constexpr local_obj(Args&&... args) 
		noexcept(has_nothrow_constructor_v<T,Args...>) 
		: bytes{ 0 }  {
		construct(util::forward<Args>(args)...);
	}

	void swap(local_obj & other)
	{
		util::swap(*address(), *other.address());
	}

	//  call ctor
	template<typename... Args>
	CCDK_FORCEINLINE void construct(Args&&... args)
		noexcept(has_nothrow_constructor_v<T, Args...>) {
		util::construct<T>(bytes, util::forward<Args>(args)...);
	}

	// call dtor
	CCDK_FORCEINLINE void destruct() noexcept {
		util::destruct<T>(bytes);
	}

	// address
	CCDK_FORCEINLINE T* address() noexcept {
		return reinterpret_cast<T*>((void*)bytes);
	}

	CCDK_FORCEINLINE T const* address() const noexcept {
		return reinterpret_cast<T const*>((void*)bytes);
	}

	// cast 
	CCDK_FORCEINLINE operator T&() noexcept { return *address(); }
	CCDK_FORCEINLINE operator T const&() const noexcept { return *address(); }

	// reference member
	CCDK_FORCEINLINE T* operator->() noexcept { return address(); }
	CCDK_FORCEINLINE T const* operator->() const noexcept { return address(); }
};

ccdk_namespace_mpl_fs_end