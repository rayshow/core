
#pragma once
#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/type_traits/is_same.h>
#include<ccdk/mpl/smart_ptr/smart_ptr_fwd.h>
#include<ccdk/mpl/smart_ptr/default_deleter.h>
#include<ccdk/mpl/smart_ptr/default_ref_count.h>

ccdk_namespace_mpl_sp_start

template<typename T>
struct has_inner_enable_share
{
	template<typename U , bool = U::share_this>
	constexpr static  bool sfinae(int) { return true; };
	template<typename U  >
	constexpr static  bool  sfinae(...) { return false; };
	static constexpr bool value = sfinae<T>(0);
};

template<typename T>
struct is_enable_share : bool_< has_inner_enable_share<T>::value > {};


template<
	typename T,
	typename D = default_deleter<T> , 
	typename R = default_ref_count<> >
class enable_share
{
public:
	typedef weak_ptr<T, D, R> weak_type;
	constexpr static  bool share_this = true;
	template<typename, typename, typename> friend class share_ptr_base;
public:
	 /* if new to share_ptr will keep a ref of this, otherwise keep null */
	  weak_type weak_ref; 
public: 

	enable_share()
	{
	}

	/* if ref_this is null will throw bad_weak_ptr */
	share_ptr<T, D, R> share_from_this() const 
	{
		return weak_ref.lock();
	}
};


ccdk_namespace_mpl_sp_end