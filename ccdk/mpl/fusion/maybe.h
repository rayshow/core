#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/base/type_.h>
#include<ccdk/mpl/base/enable_if.h>
#include<ccdk/mpl/type_traits/is_empty.h>
#include<ccdk/mpl/type_traits/is_final.h>
#include<ccdk/mpl/type_traits/is_convertible.h>
#include<ccdk/mpl/type_traits/max_aligned_storage.h>
#include<ccdk/mpl/mcontainer/val_pack.h>
#include<ccdk/mpl/util/forward.h>
#include<ccdk/mpl/util/swap.h>


ccdk_namespace_mpl_fs_start

template<typename T>
class maybe
{
public:
	static constexpr uint32 memory_size = max_aligned_storage<0,0, T>::MaxSize;
	typedef maybe              this_type;
	typedef decay_t<T>         value_type;

private:
	char  memory[memory_size];
	bool  initalized;

	template<typename P>
	CCDK_FORCEINLINE constexpr void construct(P&& p) {
		new(memory) T{ util::forward<P>(p) };
		initalized = true;
	}

	CCDK_FORCEINLINE constexpr void destruct() {
		reinterpret_cast<T>(memory).~T();
		initalized = false;
	}

public:
	/* default */
	CCDK_FORCEINLINE constexpr maybe() : initalized{ false } {}
	/* value */
	CCDK_FORCEINLINE constexpr maybe(T const& t) { construct(t); }
	CCDK_FORCEINLINE constexpr maybe(T&& t) { construct(util::move(t)); }

	/*copy and move */
	CCDK_FORCEINLINE constexpr maybe(maybe const& other) { if (other.initalized) { construct(*reinterpret_cast<T*>(other.memory)); } initalized = other.initalized; }
	CCDK_FORCEINLINE constexpr maybe(maybe&& other) { if (other.initalized) { construct(util::move(*reinterpret_cast<T*>(other.memory))); } initalized = other.initalized; }

	/*swap*/
	CCDK_FORCEINLINE constexpr void swap(maybe& other)
	{
		util::swap(*pointer(), *other.pointer());
		util::swap(initalized, other.initalized);
	}

	/* value assign */
	CCDK_FORCEINLINE maybe& operator=(ptr::nullptr_t) { maybe{}.swap(*this); }
	CCDK_FORCEINLINE maybe& operator=(T const& t) { maybe{ t }.swap(*this); }
	CCDK_FORCEINLINE maybe& operator=(T&& t) { maybe{ util::move(t) }.swap(*this); }

	/* copy and move ,avoid self assign */
	CCDK_FORCEINLINE maybe& operator=(maybe const& other) { ccdk_if_not_this(other) { maybe{ other }.swap(*this); } }
	CCDK_FORCEINLINE maybe& operator=(maybe && other) { ccdk_if_not_this(other) { maybe{ util::move(other) }.swap(*this); } }

	/* access */
	CCDK_FORCEINLINE constexpr T* pointer() const { return reinterpret_cast<T*>((void*)memory); }
	CCDK_FORCEINLINE operator bool() { return initalized; }

	CCDK_FORCEINLINE T& value() noexcept { return *pointer(); }
	CCDK_FORCEINLINE T const& value() const noexcept { return *pointer(); }
	CCDK_FORCEINLINE T& operator*() noexcept { return *pointer(); }
	CCDK_FORCEINLINE T const& operator*() const noexcept { return *pointer(); }
	CCDK_FORCEINLINE T* operator->() noexcept { return pointer(); }
	CCDK_FORCEINLINE T const* operator->() const noexcept { return pointer(); }

	CCDK_FORCEINLINE ~maybe() { if (initalized) { destruct(); } }
};


ccdk_namespace_mpl_fs_end