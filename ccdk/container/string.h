#pragma once

#include<memory>
#include<cassert>

#include"char_trait.h"
//replace and extend std::base_string<T> ,
//add more string operations

namespace Aurora3D
{
	template<typename T>
	struct TStringConvertible:public std::false_type
	{};

	template<typename element, typename allocator>
	class TBasicStringAllocator
	{
	public:
		typedef allocator allocator_type;
		typedef element   value_type;
		typedef element*  pointer;
		typedef int32     size_type;
	public:
		pointer Allocate(size_type n)
		{
			return static_cast<pointer>(allocator.allocate(n));
		}
		void    Deallocate(pointer addr)
		{
			allocator.deallocate(static_cast<void*>(addr));
		}
	private:
		allocator_type allocator;
	};


	template<typename element,
		typename trait = TCharTrait<element>,
		typename allocator = std::allocator<element> >
	class TBasicString : private TBasicStringAllocator<element, allocator>
	{
	public:
		typedef TBasicString<element, trait, allocator>  this_type;
		typedef element								     value_type;
		typedef trait									 trait_type;
		typedef allocator								 allocator_type;
		typedef int32									 size_type;
		typedef uint32									 difference_type;
		typedef element*								 pointer;
		typedef element const*							 const_pointer;
		typedef element&								 reference;
		typedef element const &							 const_reference;

		constexpr size_type NotFound = size_type(-1);
		constexpr size_type kMaxSize = size_type(-2);
		constexpr size_type kMinAllocateSize = 10;
		constexpr size_type kReallocateRatio = 2;

		//convert
		static this_type ToString(int32);
		static this_type ToString(uint32);
		static this_type ToString(int64);
		static this_type ToString(uint64);
		static this_type ToString(float);
		this_type ToString(double);
		template<typename T>
		static std::enable_if_t<TStringConvertible<T>::value, this_type>
			ToString(const T&);

		//add 
		friend reference operator+(const_reference t1, const_reference t2);
		friend reference operator+(const_reference t1, const_pointer raw);
		friend reference operator+(const_reference t1, value_type c);
		friend reference operator+(value_type c, const_reference t1);
		template<typename T>
		friend std::enable_if_t<
			std::is_arithmetic<T>::value | TStringConvertible<T>::value,
			reference>
			operator+(const_reference t1, const T& t);

		template<typename T> 
		friend std::enable_if_t<
			std::is_arithmetic<T>::value | TStringConvertible<T>::value,
			reference>
			operator+(const T& t, const_reference t1);
		//compare
		friend bool operator<(const_reference t1, const_reference t2);
		friend bool operator==(const_reference t1, const_reference t2);

	public:
		//member is trivial, so use default construction and assignment
		TBasicString() = default;
		TBasicString(const_reference) = default;
		TBasicString(this_type&&) = default;
		reference operator=(const_reference) = default;
		reference operator=(this_type&&) = default;

		//c-string-construction
		TBasicString(const_pointer str):
			length(trait_type::Length(str))
		{
			
		}
		TBasicString(const_pointer str, size_type len);
		TBasicString(value_type  c, size_type times);
		template<typename elem2, typename trait2, typename allocator2>
		TBasicString(const TBasicString<elem2, trait2, allocator2>& other);
		~TBasicString();

		//assign
		void Swap(const this_type& other) noexcept
		{

		}
		this_type& Assign(const this_type& other, size_type pos, size_type len);
		this_type& Assign(const value_type* str, size_type len);
		this_type& Assign(value_type c, size_type times);
		this_type& operator=(const value_type* str);

		//attribute
		bool Empty() const;
		size_type Length() const;
		size_type Capacity() const;
		const value_type* RawString();

		//access
		reference       operator[](size_type index = 0);
		const_reference operator[](size_type index = 0) const;
		reference       Back(size_type index = 0);
		const_reference Back(size_type index = 0) const;
		const_pointer   AddressAt(size_type index = 0) const;

		//append  string+= T1;
		this_type& operator+=(const this_type& other);
		this_type& operator+=(const value_type* str);
		this_type& operator+=(value_type c);
		template<typename T>
		 std::enable_if_t<
			std::is_arithmetic<T>::value | TStringConvertible<T>::value,
			this_type&>
			 operator+=(const T&);

		 //insert
		 this_type& Insert(size_type pos, const this_type& other);
		 this_type& Insert(size_type pos, )

		 //remove
		 void Clear();
		 void Reset();
		 this_type& Erase(size_type pos, size_type len);
		 this_type& RErase(size_type pos, size_type len);
		 this_type& PopBack();

		 //replace
		 this_type& Replace(size_type pos, size_type len,const this_type& other);
		 this_type& Replace(size_type pos, size_type len, const value_type* str, size_type len2);

		 //find
		 size_type Find(const this_type& other, size_type nth = 1);
		 size_type Find(value_type c, size_type nth = 1);
		 size_type RFind(const this_type& other, size_type nth = 1);
		 size_type RFind(value_type c, size_type nth = 1);

		 //substring
		 this_type SubString(size_type pos, size_type len);
		 this_type LeftString(size_type len);
		 this_type RightString(size_type len);

		 //transform
		 this_type& LTrim();
		 this_type& RTirm();
		 this_type& Trim();
		 this_type& ToLower();
		 this_type& ToUpper();

		 //compare
		 int32 Compare(const value_type* str, size_type len) const;
		 int32 SubCompare(size_type pos, size_type len, const this_type& other) const;
		 int32 SubCompare(size_type pos, size_type len,
			 const value_type* str, size_type len) const;

	private:
		pointer   data = nullptr;
		size_type length = 0;
		size_type capcity = 0;
	};
}
