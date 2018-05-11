#pragma once

#include<ccdk/container/container_mudule.h>
#include<ccdk/container/impl/hash_table.h>

ccdk_namespace_ct_start

using namespace mpl;

template<
	typename Key,
	typename Value,
	typename MaxLoadFactor = units::ratio<1, 2>,
	typename Size = uint32,
	typename Alloc = mem::simple_new_allocator<fs::pair<Key, Value>, Size>,
	typename Container = hash_table<
		false, Key, Value, fs::pair<Key, Value>,
		fs::get_first_t, fs::get_second_t,
		MaxLoadFactor, Size, Alloc>
>
class unordered_map: public Container
{
private:
	using Container::insert_unique;
	using Container::insert_multiple;
	using Container::emplace_unique;
	using Container::emplace_multiple;

public:
	using this_type    = unordered_map;
	using super_type   = Container;
	using size_type    = typename Container::size_type;
	using T = fs::pair<Key, Value>;
	
	using value_type      = typename Container::value_type;
	using pointer         = typename Container::pointer;
	using const_pointer   = typename Container::const_pointer;
	using reference       = typename Container::reference;
	using const_reference = typename Container::const_reference;
	using size_type       = typename Container::size_type;
	using difference_type = typename Container::difference_type;

	using Container::operator=;
	using Container::operator[];
	using Container::at;
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

///////////////////////////////////////////////////////////////////////
//// ctor 

	// ctor and nullptr-ctor
	CCDK_FORCEINLINE unordered_map() :super_type{} {}
	CCDK_FORCEINLINE unordered_map(ptr::nullptr_t) :super_type{nullptr} {}

	//copy
	CCDK_FORCEINLINE unordered_map(this_type const& other)
		: unordered_map{ other.begin(), other.size() } {}

	//move
	CCDK_FORCEINLINE unordered_map(unordered_map && other) :
		super_type{ util::move(other) } {}

	//range-n-ctor
	template<typename InputIt, typename = check_t<is_iterator<InputIt>>>
	CCDK_FORCEINLINE unordered_map(InputIt begin, size_type n)
		: super_type { n } {
		insert_unique(begin, n);
	}

	//range-ctor
	template<typename InputIt, typename = check_t<is_iterator<InputIt>>>
	CCDK_FORCEINLINE unordered_map(InputIt begin, InputIt end)
		:unordered_map{ begin, it::distance(begin, end) } {}

	// array initialize
	template<uint32 N>
	CCDK_FORCEINLINE unordered_map(T const (&arr)[N])
		: unordered_map{ arr, N } {}

///////////////////////////////////////////////////////////////////////
//// assign 

	//copy assign, avoid self assign
	CCDK_FORCEINLINE this_type& operator=(this_type const& other) {
		ccdk_if_not_this(other) {
			clear();
			insert_unique(other.cbegin(), other.size());
		}
		return *this;
	}

	//move assign,avoid self assign
	CCDK_FORCEINLINE this_type& operator=(this_type && other) {
		assign(util::move(other));
		return *this;
	}


	// literial array
	CCDK_FORCEINLINE this_type& 
	operator=(std::initializer_list<T> const& lst) {
		clear();
		insert_unique(lst.begin(), lst.size());
		return *this;
	}

	//range
	template<typename InputIt, typename = check_t<is_iterator<InputIt>>>
	CCDK_FORCEINLINE this_type& assign(InputIt begin, size_type n) {
		clear();
		insert_unique(begin, n);
		return *this;
	}

	//range-n
	template<typename InputIt, typename = check_t<is_iterator<InputIt>>>
	CCDK_FORCEINLINE this_type& assign(InputIt begin, InputIt end) {
		clear();
		insert_unique(begin, end);
		return *this;
	}


///////////////////////////////////////////////////////////////////////
//// insert / emplace 

	//emplace construct with args at key
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
};

template<
	typename Key,
	typename Value,
	typename MaxLoadFactor = units::ratio<1, 2>,
	typename Size = uint32,
	typename Alloc = mem::simple_new_allocator<fs::pair<Key, Value>>,
	typename Container = hash_table<
		true, Key, Value, fs::pair<Key, Value>,
		fs::get_first_t, fs::get_second_t,
		MaxLoadFactor, Size, Alloc>
>
class unordered_multimap : public Container
{
private:
	using Container::insert_unique;
	using Container::insert_multiple;
	using Container::emplace_unique;
	using Container::emplace_multiple;

public:
	using this_type = unordered_multimap;
	using super_type = Container;
	using size_type = typename Container::size_type;
	using T = fs::pair<Key, Value>;

	using value_type      = typename Container::value_type;
	using pointer         = typename Container::pointer;
	using const_pointer   = typename Container::const_pointer;
	using reference       = typename Container::reference;
	using const_reference = typename Container::const_reference;
	using size_type       = typename Container::size_type;
	using difference_type = typename Container::difference_type;

	using Container::operator=;
	using Container::operator[];
	using Container::at;
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

	///////////////////////////////////////////////////////////////////////
	//// ctor 

	// ctor and nullptr-ctor
	CCDK_FORCEINLINE unordered_multimap() :super_type{} {}
	CCDK_FORCEINLINE unordered_multimap(ptr::nullptr_t) : super_type{ nullptr } {}

	//copy
	CCDK_FORCEINLINE unordered_multimap(this_type const& other)
		: unordered_multimap{ other.begin(), other.size() } {}

	//move
	CCDK_FORCEINLINE unordered_multimap(this_type && other) :
		super_type{ util::move(other) } {}

	//range-n-ctor
	template<typename InputIt, typename = check_t<is_iterator<InputIt>>>
	CCDK_FORCEINLINE unordered_multimap(InputIt begin, size_type n)
		: super_type{ n } {
		insert_multiple(begin, n);
	}

	//range-ctor
	template<typename InputIt, typename = check_t<is_iterator<InputIt>>>
	CCDK_FORCEINLINE unordered_multimap(InputIt begin, InputIt end)
		:unordered_multimap{ begin, it::distance(begin, end) } {}

	// array initialize
	template<uint32 N>
	CCDK_FORCEINLINE unordered_multimap(T const (&arr)[N])
		: unordered_multimap{ arr, N } {}

	///////////////////////////////////////////////////////////////////////
	//// assign 

	//copy assign, avoid self assign
	CCDK_FORCEINLINE this_type& operator=(this_type const& other) {
		ccdk_if_not_this(other) {
			clear();
			insert_multiple(other.cbegin(), other.size());
		}
		return *this;
	}

	//move assign,avoid self assign
	CCDK_FORCEINLINE this_type& operator=(this_type && other) {
		assign(util::move(other));
		return *this;
	}


	// literial array
	CCDK_FORCEINLINE this_type&
		operator=(std::initializer_list<T> const& lst) {
		clear();
		insert_multiple(lst.begin(), lst.size());
		return *this;
	}

	//range
	template<typename InputIt, typename = check_t<is_iterator<InputIt>>>
	CCDK_FORCEINLINE this_type& assign(InputIt begin, size_type n) {
		clear();
		insert_multiple(begin, n);
		return *this;
	}

	//range-n
	template<typename InputIt, typename = check_t<is_iterator<InputIt>>>
	CCDK_FORCEINLINE this_type& assign(InputIt begin, InputIt end) {
		clear();
		insert_multiple(begin, end);
		return *this;
	}


	///////////////////////////////////////////////////////////////////////
	//// insert / emplace 

	//emplace construct with args at key
	template<
		typename... Args,
		typename = check_t< has_constructor<T, Args...>> >
		CCDK_FORCEINLINE auto emplace(Args&&... args) {
		return emplace_multiple(util::forward<Args>(args)...);
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
		std::initializer_list<T> const& list) {
		return insert_multiple(list);
	}

/////////////////////////////////////////////////////////////////////////
//// for debug

	CCDK_FORCEINLINE void debug_value() const noexcept {
		foreach([](T const& t) {
			DebugValueIt(t.first, t.second);
		});
		DebugValueItEnd();
	}

};


ccdk_namespace_ct_end