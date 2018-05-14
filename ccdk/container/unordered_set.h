#pragma once
#include<ccdk/mpl/function/identity.h>
#include<ccdk/container/container_mudule.h>
#include<ccdk/container/impl/hash_table.h>

ccdk_namespace_ct_start

using namespace mpl;

template<
	typename T,
	typename MaxLoadFactor = units::ratio<1, 2>,
	typename Size = uint32,
	typename Alloc = mem::simple_new_allocator<T,Size>,
	typename Container = hash_table<
		false, T, T, T, fn::identity_t, fn::identity_t,
		MaxLoadFactor, Size, Alloc>
>
class unordered_set : public Container
{
private: 
	using Container::operator[];
	using Container::at;
	using Container::insert_unique;
	using Container::insert_multiple;
	using Container::emplace_unique;
	using Container::emplace_multiple;

public:
	using this_type  = unordered_set;
	using super_type = Container;
	using size_type  = Size;

	using Container::assign;
	using Container::swap;
	using Container::erase;
	using Container::clear;
	using Container::find;
	using Container::exists;
	using Container::size;
	using Container::empty;
	using Container::max_size;
	using Container::bucket_size;
	using Container::load_factor;
	using Container::begin;
	using Container::end;
	using Container::cbegin;
	using Container::cend;
	using Container::rehash;
	using Container::foreach;
	using Container::clone;
	using Container::conflict_count;

///////////////////////////////////////////////////////////////////////////
//// ctor

	//default and nullptr ctor
	CCDK_FORCEINLINE unordered_set() 
		noexcept : super_type{} {}
	CCDK_FORCEINLINE unordered_set(ptr::nullptr_t) 
		noexcept : super_type{nullptr} {}

	//range-n
	template<typename InputIt, typename = check_t<is_iterator<InputIt>>>
	CCDK_FORCEINLINE unordered_set(InputIt begin, size_type n)
		: super_type{ n } { insert_unique(begin, n); }

	//range
	template<typename InputIt, typename = check_t<is_iterator<InputIt>>>
	CCDK_FORCEINLINE unordered_set(InputIt begin, InputIt end)
		: unordered_set{ begin, it::distance(begin, end) } {}

	// array initialize
	template<uint32 N>
	CCDK_FORCEINLINE unordered_set(T const (&arr)[N])
		: unordered_set{ arr, N } {}

	//copy ctor
	CCDK_FORCEINLINE unordered_set(unordered_set const& other)
		: unordered_set{ other.begin(), other.size() } {}

	//move ctor
	CCDK_FORCEINLINE unordered_set(unordered_set&& other)
		: super_type{ util::move(other) } {}

///////////////////////////////////////////////////////////////////////////
//// assign

	//clear all element
	CCDK_FORCEINLINE this_type& operator=(ptr::nullptr_t) noexcept { clear(); }

	//move assign, avoid self assign
	CCDK_FORCEINLINE this_type& operator=(this_type&& other) noexcept {
		assign(util::move(other));
		return *this;
	}

	//copy assign, avoid self assign
	CCDK_FORCEINLINE this_type& operator=(this_type const& other) {
		ccdk_if_not_this(other) {
			clear();
			insert_unique(other.cbegin(), other.size());
		}
		return *this;
	}

	//assign array
	CCDK_FORCEINLINE this_type& operator=(std::initializer_list<T> const& lst) {
		clear();
		insert_unique(lst.begin(), lst.end());
		return *this;
	}

	//range assign 
	template<typename InputIt, typename = check_t<is_iterator<InputIt>>>
	CCDK_FORCEINLINE this_type& assign(InputIt begin, InputIt end) {
		clear();
		insert_unique(begin, end);
		return *this;
	}

	//range-n assign 
	template<typename InputIt, typename = check_t<is_iterator<InputIt>>>
	CCDK_FORCEINLINE this_type& assign(InputIt begin, size_type n) {
		clear();
		insert_unique(begin, n);
		return *this;
	}

///////////////////////////////////////////////////////////////////////////
//// insert / emplace

	//emplace construct 
	template<
		typename... Args,
		typename = check_t< has_constructor<T, Args...>> >
	CCDK_FORCEINLINE auto emplace(Args&&... args) {
		return emplace_unique(util::forward<Args>(args)...);
	}

	// insert a element
	CCDK_FORCEINLINE auto insert(T const& t) { return insert_unique(t); }

	// insert a r-value element
	CCDK_FORCEINLINE auto insert(T && t) { return insert_unique(util::move(t)); }

	// insert range-n
	template<
		typename InputIt,
		typename = check_t<is_iterator<InputIt>> >
		CCDK_FORCEINLINE auto insert(InputIt begin, size_type n) {
		return insert_unique(begin, n);
	}

	// insert range
	template<
		typename InputIt,
		typename = check_t<is_iterator<InputIt>> >
		CCDK_FORCEINLINE auto insert(InputIt begin, InputIt end) {
		return insert_unique(begin, end);
	}

	//insert initialize_list
	CCDK_FORCEINLINE auto insert(
		std::initializer_list<T> const& list) {
		return insert_unique(list);
	}


///////////////////////////////////////////////////////////////////////////
//// set operation

	// intersect with other set, generate a new set contain the common elements 
	CCDK_FORCEINLINE this_type intersect(this_type const& other) const {
		this_type ret{};
		//choose shorter set to loop over
		if (size() < other.size()) {
			this->foreach([&](const T& d) { if (other.exists(d)) { ret.insert(d); } });
		}
		else {
			other.foreach([&](const T& d) { if (exists(d)) { ret.insert(d); } });
		}
		return ret;
	}

	// union with other set, generate a new set contain both elements 
	CCDK_FORCEINLINE this_type unions(this_type const& other) const {
		this_type ret{};
		this->foreach([&](const T& d) { ret.insert(d); });
		other.foreach([&](const T& d) { ret.insert(d); });
		return ret;
	}

	// this set - other set , generate a new set contain elements 
	// in this but not in other
	CCDK_FORCEINLINE this_type minus(this_type const& other) const {
		this_type ret{};
		this->foreach([&](const T& d) { if (!other.exists(d)) { ret.insert(d); } });
		return ret;
	}

	// symbolic of intersect
	CCDK_FORCEINLINE this_type& operator&(this_type const& other)
		const { return intersect(other); }

	// symbolic unions
	CCDK_FORCEINLINE this_type& operator|(this_type const& other) const {
		return unions(other);
	}

	// symbolic minus
	CCDK_FORCEINLINE this_type operator-(this_type const& other) const {
		return minus(other);
	}

///////////////////////////////////////////////////////////////////////
//// 

	CCDK_FORCEINLINE void debug_value(const char* title="") const noexcept {
		DebugValueItBegin(title);
		foreach([](T const& t) {
			DebugValueIt(t);
		});
		DebugValueItEnd();
	}

	CCDK_FORCEINLINE void debug_it()  noexcept {
		for (auto it = begin(); it != end(); ++it) {
			DebugValueIt(*it);
		}
		DebugValueItEnd();
	}

};



template<
	typename T,
	typename MaxLoadFactor = units::ratio<1, 2>,
	typename Size = uint32,
	typename Alloc = mem::simple_new_allocator<T,Size>,
	typename Container = hash_table<
	true, T, T, T, fn::identity_t, fn::identity_t,
	MaxLoadFactor, Size, Alloc>
>
class unordered_multiset : public Container
{
private:
	using Container::operator[];
	using Container::at;
	using Container::insert_unique;
	using Container::insert_multiple;
	using Container::emplace_unique;
	using Container::emplace_multiple;

public:
	using this_type = unordered_multiset;
	using super_type = Container;
	using size_type = Size;

	using Container::assign;
	using Container::swap;
	using Container::erase;
	using Container::clear;
	using Container::find;
	using Container::exists;
	using Container::size;
	using Container::empty;
	using Container::max_size;
	using Container::bucket_size;
	using Container::load_factor;
	using Container::begin;
	using Container::end;
	using Container::cbegin;
	using Container::cend;
	using Container::rehash;
	using Container::foreach;
	using Container::clone;

	///////////////////////////////////////////////////////////////////////////
	//// ctor

	//default and nullptr ctor
	CCDK_FORCEINLINE unordered_multiset() noexcept :super_type{} {}
	CCDK_FORCEINLINE unordered_multiset(ptr::nullptr_t)
		noexcept : super_type{ nullptr } {}

	//range-n
	template<typename InputIt, typename = check_t<is_iterator<InputIt>>>
	CCDK_FORCEINLINE unordered_multiset(InputIt begin, size_type n)
		: super_type{ n } {
		insert_multiple(begin, n);
	}

	//range
	template<typename InputIt, typename = check_t<is_iterator<InputIt>>>
	CCDK_FORCEINLINE unordered_multiset(InputIt begin, InputIt end)
		: unordered_multiset{ begin, it::distance(begin, end) } {}

	// array initialize
	template<uint32 N>
	CCDK_FORCEINLINE unordered_multiset(T const (&arr)[N])
		: unordered_multiset{ arr, N } {}

	//copy ctor
	CCDK_FORCEINLINE unordered_multiset(this_type const& other)
		: unordered_multiset{ other.begin(), other.size() } {}

	//move ctor
	CCDK_FORCEINLINE unordered_multiset(this_type&& other)
		: super_type{ util::move(other) } {}

	///////////////////////////////////////////////////////////////////////////
	//// assign

	//clear all element
	CCDK_FORCEINLINE this_type& operator=(ptr::nullptr_t) noexcept { clear(); }

	//move assign, avoid self assign
	CCDK_FORCEINLINE this_type& operator=(this_type&& other) noexcept {
		assign(util::move(other));
		return *this;
	}

	//copy assign, avoid self assign
	CCDK_FORCEINLINE this_type& operator=(this_type const& other) {
		ccdk_if_not_this(other) {
			clear();
			insert_multiple(other.cbegin(), other.size());
		}
		return *this;
	}

	//assign array
	CCDK_FORCEINLINE this_type& operator=(std::initializer_list<T> const& lst) {
		clear();
		insert_multiple(lst.begin(), lst.end());
		return *this;
	}

	//range assign 
	template<typename InputIt, typename = check_t<is_iterator<InputIt>>>
	CCDK_FORCEINLINE this_type& assign(InputIt begin, InputIt end) {
		clear();
		insert_multiple(begin, end);
		return *this;
	}

	//range-n assign 
	template<typename InputIt, typename = check_t<is_iterator<InputIt>>>
	CCDK_FORCEINLINE this_type& assign(InputIt begin, size_type n) {
		clear();
		insert_multiple(begin, n);
		return *this;
	}

	///////////////////////////////////////////////////////////////////////////
	//// insert / emplace

	//emplace construct 
	template<
		typename... Args,
		typename = check_t< has_constructor<T, Args...>> >
		CCDK_FORCEINLINE auto emplace(Args&&... args) {
		return insert_multiple(util::forward<Args>(args)...);
	}

	// insert a element
	CCDK_FORCEINLINE auto insert(T const& t) { return insert_multiple(t); }

	// insert a r-value element
	CCDK_FORCEINLINE auto insert(T && t) { return insert_multiple(util::move(t)); }

	// insert range-n
	template<
		typename InputIt,
		typename = check_t<is_iterator<InputIt>> >
		CCDK_FORCEINLINE auto insert(InputIt begin, size_type n) {
		return insert_multiple(begin, n);
	}

	// insert range
	template<
		typename InputIt,
		typename = check_t<is_iterator<InputIt>> >
		CCDK_FORCEINLINE auto insert(InputIt begin, InputIt end) {
		return insert_multiple(begin, end);
	}

	//insert initialize_list
	CCDK_FORCEINLINE auto insert(
		std::initializer_list<T> const& lst) {
		return insert_multiple(lst);
	}


	///////////////////////////////////////////////////////////////////////////
	//// set operation

	// intersect with other set, generate a new set contain the common elements 
	CCDK_FORCEINLINE this_type intersect(this_type const& other) const {
		this_type ret{};
		//choose shorter set to loop over
		if (size() < other.size()) {
			this->foreach([&](const T& d) { if (other.exists(d)) { ret.insert(d); } });
		}
		else {
			other.foreach([&](const T& d) { if (exists(d)) { ret.insert(d); } });
		}
		return ret;
	}

	// union with other set, generate a new set contain both elements 
	CCDK_FORCEINLINE this_type unions(this_type const& other) const {
		this_type ret{};
		this->foreach([&](const T& d) { ret.insert(d); });
		other.foreach([&](const T& d) { ret.insert(d); });
		return ret;
	}

	// this set - other set , generate a new set contain elements 
	// in this but not in other
	CCDK_FORCEINLINE this_type minus(this_type const& other) const {
		this_type ret{};
		this->foreach([&](const T& d) { if (!other.exists(d)) { ret.insert(d); } });
		return ret;
	}

	// symbolic of intersect
	CCDK_FORCEINLINE this_type& operator&(this_type const& other)
		const {
		return intersect(other);
	}

	// symbolic unions
	CCDK_FORCEINLINE this_type& operator|(this_type const& other) const {
		return unions(other);
	}

	// symbolic minus
	CCDK_FORCEINLINE this_type operator-(this_type const& other) const {
		return minus(other);
	}

	///////////////////////////////////////////////////////////////////////
	//// 

	CCDK_FORCEINLINE void debug_value(const char* title = "") const noexcept {
		DebugValueItBegin(title);
		foreach([](T const& t) {
			DebugValueIt(t);
		});
		DebugValueItEnd();
	}

	CCDK_FORCEINLINE void debug_it()  noexcept {
		for (auto it = begin(); it != end(); ++it) {
			DebugValueIt(*it);
		}
		DebugValueItEnd();
	}

};


ccdk_namespace_ct_end