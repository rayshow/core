#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/type_traits/is_convertible.h>
#include<ccdk/mpl/type_traits/is_pod.h>
#include<ccdk/mpl/type_traits/is_compatible.h>
#include<ccdk/mpl/base/enable_if.h>
#include<ccdk/mpl/util/noncopyable.h>
#include<ccdk/mpl/util/move.h>
#include<ccdk/mpl/util/swap.h>
#include<ccdk/mpl/util/addressof.h>
#include<ccdk/mpl/smart_ptr/smart_ptr_fwd.h>
#include<ccdk/mpl/smart_ptr/default_deleter.h>

ccdk_namespace_mpl_sp_start

template<
	typename Type,
	typename Deleter
>
struct unique_ptr_base  :public util::noncopyable
{
	typedef unique_ptr_base this_type;
	typedef Type		    value_type;
	typedef Type*           pointer_type;
	typedef Deleter         deleter_type;

private:
	pointer_type content;

public:
	/* default and nullptr constructor */
	CCDK_FORCEINLINE unique_ptr_base() noexcept : content{ nullptr } {}
	CCDK_FORCEINLINE unique_ptr_base(ptr::nullptr_t) noexcept : content{ nullptr } {}

	/* pointer constructor */
	CCDK_FORCEINLINE unique_ptr_base(pointer_type ptr) noexcept : content(ptr) {}

	/* move from compatible unique_ptr_base<T2,D2> */
	template<typename T2, typename D2, typename = check_t< is_convertible<unique_ptr_base<T2, D2>,this_type> > >
	CCDK_FORCEINLINE unique_ptr_base(unique_ptr_base<T2,D2>&& other) noexcept : content{ other.content } { other.content = nullptr; }

	/* swap compatible unique_ptr_base<T2,D2> */
	template<typename T2, typename D2, typename = check_t< is_compatible<unique_ptr_base<T2, D2>, this_type> > >
	CCDK_FORCEINLINE void swap(unique_ptr_base<T2,D2>& other) noexcept { util::swap(content, other.content); }

	/* pointer assign, release ahead pointer may throw */
	CCDK_FORCEINLINE unique_ptr_base& operator=(pointer_type ptr)  { unique_ptr_base{ ptr }.swap(*this); return *this; }

	/* move assign , prevent self move */
	CCDK_FORCEINLINE unique_ptr_base& operator=(unique_ptr_base&& other) { ccdk_if_not_this(other) { unique_ptr_base{ util::move(other) }.swap(*this); } return *this; }
	template<typename T2, typename = check_t< is_convertible<T2*, T*> > >
	CCDK_FORCEINLINE unique_ptr_base& operator=(unique_ptr_base<T2>&& other) { unique_ptr_base{ util::move(other) }.swap(*this); return *this; }

	//non-copy
	template<typename T2> unique_ptr_base(const unique_ptr_base<T2>&) = delete;
	template<typename T2> unique_ptr_base& operator=(const unique_ptr_base<T2>&) = delete;

	//get pointer
	CCDK_FORCEINLINE pointer_type pointer() const noexcept { return content; }

	//release
	CCDK_FORCEINLINE void release() { if(ccdk_likely(content)) Deleter{}(content); }

	//delete
	CCDK_FORCEINLINE ~unique_ptr_base() { release(); }
};


template<
	typename Type,
	typename Deleter =  default_deleter<Type>
>
class unique_ptr : public unique_ptr_base<Type, Deleter>
{
public:
	typedef unique_ptr_base<Type, Deleter>   base_type;
	typedef typename base_type::pointer_type pointer_type;

	//refer member
	CCDK_FORCEINLINE pointer_type operator->() const noexcept { return pointer(); }

	//dereference, for efficient reason, assert not nullptr
	CCDK_FORCEINLINE add_lref_t<Type>       operator*()  noexcept       { ccdk_assert(pointer() != nullptr);  return *pointer(); }
	CCDK_FORCEINLINE add_const_lref_t<Type> operator*() const  noexcept { ccdk_assert(pointer() != nullptr);  return *pointer(); }
	
};

//for array
// have index
template<typename Type, typename Deleter>
struct unique_ptr<Type[], Deleter> : public unique_ptr_base<Type[], Deleter>
{
public:
	typedef nique_ptr_base<Type[], Deleter> base_type;
	
	using base_type::base_type;
	using base_type::operator=;
	using base_type::swap;
	using base_type::pointer;
	using base_type::release;
	
	//index
	CCDK_FORCEINLINE add_lref_t<Type>       operator[](int index) noexcept       { ccdk_assert(pointer() != nullptr);  return pointer()[index]; }
	CCDK_FORCEINLINE add_const_lref_t<Type> operator[](int index) const noexcept { ccdk_assert(pointer() != nullptr);  return pointer()[index]; }

};

//forbidden void content
template<> struct unique_ptr<void> : public util::noncopyable { public: unique_ptr() = delete; };

//halp fn
template<typename Type1, typename Type2, typename Deleter1, typename Deleter2> 
void swap(unique_ptr<Type1,Deleter1>& lh, unique_ptr<Type2, Deleter2>& rh) { lh.swap(rh); }
template<typename Type, typename Deleter> decltype(auto) value(const unique_ptr<Type, Deleter>& sp) { return sp.pointer(); }

//equal
template<typename Type, typename Deleter> CCDK_FORCEINLINE bool operator==(const unique_ptr<Type, Deleter>& sp, ptr::nullptr_t) { return sp.pointer() == nullptr; }
template<typename Type, typename Deleter> CCDK_FORCEINLINE bool operator==(ptr::nullptr_t, const unique_ptr<Type, Deleter>& sp) { return sp.pointer() == nullptr; }
template<typename Type1, typename Type2, typename Deleter1, typename Deleter2>
CCDK_FORCEINLINE  bool operator==(const unique_ptr<Type1, Deleter1>& lh, const unique_ptr<Type2, Deleter2>& rh) { return lh.pointer() == rh.pointer(); }

//less
template<typename Type, typename Deleter> CCDK_FORCEINLINE  bool operator<(ptr::nullptr_t, const unique_ptr<Type, Deleter>& sp) { return nullptr < sp.pointer(); }
template<typename Type, typename Deleter> CCDK_FORCEINLINE  bool operator<(const unique_ptr<Type, Deleter>& sp, ptr::nullptr_t) { return sp.pointer() < nullptr; }
template<typename Type1, typename Type2, typename Deleter1, typename Deleter2>
CCDK_FORCEINLINE  bool operator<(const unique_ptr<Type1, Deleter1>& lh, const unique_ptr<Type2, Deleter2>& rh) { return lh.pointer() < rh.pointer(); }

ccdk_namespace_mpl_sp_end