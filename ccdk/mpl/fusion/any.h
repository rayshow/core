#pragma once

#include<typeinfo>
#include<exception>

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/type_traits/decay.h>
#include<ccdk/mpl/util/forward.h>
#include<ccdk/mpl/util/swap.h>
#include<ccdk/mpl/fusion/bad_cast_exception.h>

ccdk_namespace_mpl_fs_start

struct base_holder
{
	virtual base_holder* clone() const noexcept = 0;
	virtual const std::type_info& type() const noexcept = 0;

};

template<typename T>
struct any_holder :
	public base_holder
{
	typedef decay_t<T> value_type;
private:
	value_type val;

public:

	CCDK_FORCEINLINE
	any_holder(T&& t)
		:val{ util::forward<T>(t) }
	{
		DebugFunctionName();
		DebugTypeName<value_type>();
	}

	CCDK_FORCEINLINE
	any_holder(const any_holder& other)
		:val{ other.val }
	{}

	CCDK_FORCEINLINE
	any_holder(any_holder&& other)
		:val{ util::move(other.val) }
	{}

	CCDK_FORCEINLINE
	virtual base_holder* 
		clone() const  noexcept override
	{
		return new(ptr::nothrow) any_holder<T>{ *this };
	}

	CCDK_FORCEINLINE
	virtual const std::type_info& 
		type() const noexcept override
	{
		return typeid(val);
	}

	CCDK_FORCEINLINE
	const value_type& 
		get() noexcept
	{
		return val;
	}

	CCDK_FORCEINLINE
	const value_type* 
		get_pointer() noexcept
	{
		return &val;
	}

};


struct any
{
private:
	base_holder *holder;
public:

	CCDK_FORCEINLINE
		any() noexcept
		: holder{ nullptr }
	{}

	template<typename T>
	CCDK_FORCEINLINE
		any(T&& t) noexcept
		: holder(new(ptr::nothrow) any_holder<T>(util::forward<T>(t)))
	{}

	CCDK_FORCEINLINE
		any(const any& other) 
		: holder{ other.holder ? other.holder->clone() : nullptr }
	{}

	CCDK_FORCEINLINE
		any(any&& other) noexcept
		: holder{ other.holder }
	{
		other.holder = nullptr;
	}

	
	CCDK_FORCEINLINE
		void swap(const any& other) noexcept
	{
		util::swap(holder, other.holder);
	}

	template<typename T>
	CCDK_FORCEINLINE
		any& operator=(T&& t)
	{
		any{ util::forward<T>(t) }.swap(*this);
		return *this;
	}

	CCDK_FORCEINLINE 
		any& operator=(const any& other)
	{
		any{ other }.swap(*this);
		return *this;
	}

	CCDK_FORCEINLINE
		any& operator=(any&& other) noexcept
	{
		holder = other.holder;
		other.holder = nullptr;
		return *this;
	}

	CCDK_FORCEINLINE
		operator bool() const
	{
		return !!holder;
	}

	CCDK_FORCEINLINE
		bool empty() const
	{
		return !holder;
	}

	CCDK_FORCEINLINE
		void clear() const
	{
		any{}.swap(*this);
	}

	CCDK_FORCEINLINE
		const std::type_info& type() const
	{
		return holder->type();
	}


	template<typename T>
	CCDK_FORCEINLINE
		T& to() &
	{
		return const_cast<T&>( __to_ref_impl<T>() );
	}

	template<typename T>
	CCDK_FORCEINLINE
		const T& to() const &
	{
		return __to_ref_impl<T>();
	}

	template<typename T>
	CCDK_FORCEINLINE
		T&& to() &&
	{
		return const_cast<T&&>( __to_ref_impl<T>() );
	}



	template<typename T>
	CCDK_FORCEINLINE
		T* to_pointer() noexcept
	{
		return const_cast<T*>( __to_pointer_impl<T>() );
	}

	template<typename T>
	CCDK_FORCEINLINE
		const T* to_pointer() const noexcept
	{
		return __to_pointer_impl<T>();
	}
	
	CCDK_FORCEINLINE
	~any()
	{
		ptr::safe_delete(holder);
	}

private:

	template<typename T, typename P = decay_t<T> >
	CCDK_FORCEINLINE
		const P& __to_ref_impl() const
	{
		//cv is ingored of typeid check
		if (holder->type() == typeid(P))
		{
			return static_cast< any_holder<P>*>(holder)->get();
		}
		throw bad_cast_exception{ "bad any cast at ccdk::mpl::fusion::any::__to_ref_impl<T>()" };
	}

	template<typename T, typename P = decay_t<T>>
	CCDK_FORCEINLINE
		const P* __to_pointer_impl() const noexcept
	{
		//cv is ingored of typeid check
		if (holder->type() == typeid(P))
		{
			return static_cast< any_holder<P>*>(holder)->get_pointer();
		}
		throw bad_cast_exception{ "bad any cast at ccdk::mpl::fusion::any::__to_pointer_impl<T>()" };
	}
};


ccdk_namespace_mpl_fs_end