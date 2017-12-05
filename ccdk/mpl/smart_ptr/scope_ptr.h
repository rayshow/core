#pragma once
#include<ccdk/type.h>
#include<ccdk/mpl/type_traits/is_base_of.h>
#include<ccdk/mpl/type_traits/has_destructor.h>
#include<ccdk/mpl/base/enable_if.h>
#include<ccdk/mpl/base/noncopyable.h>

namespace ccdk
{
	namespace mpl
	{
		template<typename T>
		struct scope_ptr
			:noncopyable< scope_ptr<T> >
		{
		public:
			typedef scope_ptr<T> type;
			typedef T* value_type;

		private:
			value_type& t; //set original pointer to nullptr

		public:
			scope_ptr(value_type& inT) noexcept
				: t(inT)
			{}

			//P is derive from T
			template<typename P, typename = check_t< is_child_of<P, T>>>
			scope_ptr( P* inP) noexcept
				: t(inP)
			{}

			//non-copy
			template<typename P> scope_ptr(const scope_ptr<P>&) = delete;
			template<typename P> scope_ptr& operator=(const scope_ptr<P>&) = delete;

			//refer member
			T* operator->() noexcept { return t; }
			const T* operator->() const noexcept { return t; }

			//dereference
			T& operator*() & noexcept { return *t; }
			const T& operator*() const & noexcept { return *t; }
			T&& operator*() && noexcept{ return *t; }

			//delete
			~scope_ptr() noexcept(has_nothrow_destructor_v<T>)
			{
				ptr::safe_delete(t);
			}
		};


		template<typename T> 
		struct scope_ptr<T[]>
			: noncopyable< scope_ptr<T[]> >
		{
			typedef scope_ptr<T[]> type;
			typedef T* value_type;
		private:
			value_type& t; //set original pointer to nullptr
		public:
			scope_ptr(T*& inT) noexcept
				: t(inT)
			{}

			//P is derive from T
			template<typename P, typename = check_t< is_child_of<P, T>>>
			scope_ptr(P*& inP) noexcept
				: t(inP)
			{}

			//non-copy
			template<typename P> scope_ptr(const scope_ptr<P>&) = delete;
			template<typename P> scope_ptr& operator=(const scope_ptr<P>&) = delete;

			//index
			T& operator[](int index)& { return t[index]; }
			const T& operator[](int index) const & { return t[index]; }
			T&& operator[](int index) && { return t[index]; }

			//delete
			~scope_ptr() noexcept(has_nothrow_destructor_v<T>)
			{
				ptr::safe_delete_array(t);
			}
		};
	}
}