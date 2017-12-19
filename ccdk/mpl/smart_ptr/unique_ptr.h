#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/type_traits/is_base_of.h>
#include<ccdk/mpl/base/enable_if.h>
#include<ccdk/mpl/util/noncopyable.h>

ccdk_namespace_mpl_sp_start

template<typename T>
struct unique_ptr :util::noncopyable
{
	typedef unique_ptr type;
	typedef T* value_type;
private:
	value_type& t; //set original pointer to nullptr

public:
	unique_ptr(value_type& inT) noexcept
		: t(inT)
	{}

	//P is derive from T
	template<
		typename P,
		typename = check_t< is_child_of<P, T>>
	>
		unique_ptr(P* inP) noexcept
		: t(inP)
	{}

	//non-copy
	template<typename P> unique_ptr(const unique_ptr<P>&) = delete;
	template<typename P> unique_ptr& operator=(const unique_ptr<P>&) = delete;

	//refer member
	T* operator->() noexcept { return t; }
	const T* operator->() const noexcept { return t; }

	//dereference
	T& operator*() & noexcept { return *t; }
	const T& operator*() const & noexcept { return *t; }
	T&& operator*() && noexcept{ return *t; }

	//delete
	~unique_ptr()
	{
		ptr::safe_delete(t);
	}
};


//for array
template<typename T>
struct unique_ptr<T[]> : util::noncopyable
{
	typedef unique_ptr<T[]> type;
	typedef T* value_type;
private:
	value_type& t; //set original pointer to nullptr
public:
	unique_ptr(T*& inT) noexcept
		: t(inT)
	{}

	//P is derive from T
	template<
		typename P,
		typename = check_t< is_child_of<P, T>>
	>
		unique_ptr(P*& inP) noexcept
		: t(inP)
	{}

	//non-copy
	template<typename P> unique_ptr(const unique_ptr<P>&) = delete;
	template<typename P> unique_ptr& operator=(const unique_ptr<P>&) = delete;

	//index
	T& operator[](int index)& { return t[index]; }
	const T& operator[](int index) const & { return t[index]; }
	T&& operator[](int index) && { return t[index]; }

	//deletes
	~unique_ptr()
	{
		ptr::safe_delete_array(t);
	}
};
ccdk_namespace_mpl_sp_end