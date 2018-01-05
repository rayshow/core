#pragma once

#include<typeinfo>
#include<exception>
#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/type_traits/decay.h>
#include<ccdk/mpl/util/forward.h>
#include<ccdk/mpl/util/swap.h>
#include<ccdk/mpl/util/addressof.h>
#include<ccdk/mpl/fusion/bad_cast_exception.h>

ccdk_namespace_mpl_fs_start

struct base_holder
{
	virtual base_holder* clone() const  = 0;
	virtual const std::type_info& type() const noexcept = 0;
	virtual ~base_holder() {};
};

template<typename T>
class any_holder : public base_holder
{
	typedef decay_t<T> value_type;
private:
	value_type val;

public:

	/*value constructor, copy may throw */
	CCDK_FORCEINLINE any_holder(T&& t) :val{ util::forward<T>(t) } { }

	/*copy constructor, copy constructor may throw*/
	CCDK_FORCEINLINE any_holder(const any_holder& other) :val{ other.val } {}

	/*move constructor,  move constructor may throw may throw*/
	CCDK_FORCEINLINE any_holder(any_holder&& other) :val{ util::move(other.val) } {}

	/*poly-copy, may throw when new */
	CCDK_FORCEINLINE virtual base_holder* clone() const  override { return new any_holder<T>{ *this }; }

	/* value typeid */
	CCDK_FORCEINLINE virtual const std::type_info&  type() const noexcept override { return typeid(val); }

	/* ret value */
	CCDK_FORCEINLINE  value_type& get()  noexcept  { return val; }

	/* ret value pointer */
	CCDK_FORCEINLINE  value_type*  get_pointer()  { return util::addressof(val); }

	virtual ~any_holder() {};
};


struct any
{
private:
	base_holder *holder;

	template<typename T, typename P = decay_t<T>>
	CCDK_FORCEINLINE P* _to_pointer_impl() const { if (holder->type() == typeid(P)) { return static_cast<any_holder<P>*>(holder)->get_pointer(); } ccdk_throw(bad_any_cast{}); }

	template<typename T, typename P = decay_t<T> >
	CCDK_FORCEINLINE P& _to_ref_impl() const { if (holder->type() == typeid(P)) { return static_cast< any_holder<P>*>(holder)->get(); } ccdk_throw(bad_any_cast{}); }

public:

	/*default constructor*/
	CCDK_FORCEINLINE any() noexcept : holder{ nullptr } {}

	/*value constructor, may throw */
	template<typename T>
	CCDK_FORCEINLINE explicit any(T&& t) { ccdk_safe_delete_if_exception(holder, (new any_holder<T>(util::forward<T>(t)))); }

	/*copy constructor, may throw */
	CCDK_FORCEINLINE any(const any& other) { ccdk_safe_delete_if_exception(holder, (other.holder ? other.holder->clone():nullptr)); }

	/*move constructor, pointer*/
	CCDK_FORCEINLINE any(any&& other) noexcept: holder{ other.holder } { other.holder = nullptr; }

	/* swap */
	CCDK_FORCEINLINE void swap( any& other) noexcept { util::swap(holder, other.holder); }

	/* value assign , may throw */
	template<typename T>
	CCDK_FORCEINLINE any& operator=(T&& t) { any{ util::forward<T>(t) }.swap(*this); return *this; }

	/* copy assign, copy may throw */
	CCDK_FORCEINLINE any& operator=(const any& other) { any{ other }.swap(*this); return *this; }

	/* move assign*/
	CCDK_FORCEINLINE any& operator=(any&& other) noexcept { any{ util::move(other) }.swap(other); return *this; }

	/* bool, whether hold value  */
	CCDK_FORCEINLINE explicit operator bool() const { return !!holder; }

	/* bool, not hold value */
	CCDK_FORCEINLINE bool empty() const { return !holder; }

	/* clear content */
	CCDK_FORCEINLINE void reset() noexcept { any{}.swap(*this); }

	/* typeinfo */
	CCDK_FORCEINLINE const std::type_info& typeinfo() const { return holder->type(); }

	template<typename T>
	CCDK_FORCEINLINE T& to()  { return _to_ref_impl<T>(); }

	template<typename T>
	CCDK_FORCEINLINE const T& to() const  { return _to_ref_impl<T>(); }

	template<typename T>
	CCDK_FORCEINLINE T* to_pointer() { return _to_pointer_impl<T>(); }

	template<typename T>
	CCDK_FORCEINLINE const T* to_pointer() const  { return _to_pointer_impl<T>(); }
	
	CCDK_FORCEINLINE ~any() { ptr::safe_delete(holder); }
};


ccdk_namespace_mpl_fs_end