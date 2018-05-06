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
	typename Alloc = mem::simple_new_allocator<T>,
	typename Container = hash_table<
		T, T, T, fn::identity_t, fn::identity_t,
		MaxLoadFactor, Size, Alloc>
>
class unordered_set : public Container
{
private: 
	using Container::operator[];
	using Container::at;

public:
	using this_type  = unordered_set;

	using Container::Container;
	using Container::operator=;
	using Container::assign;
	using Container::swap;
	using Container::emplace;
	using Container::insert;
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

	CCDK_FORCEINLINE this_type unions(this_type const& other) const {
		this_type ret{};
		this->foreach([&](const T& d) { ret.insert(d); });
		other.foreach([&](const T& d) { ret.insert(d); });
		return ret;
	}

	CCDK_FORCEINLINE this_type minus(this_type const& other) const {
		this_type ret{};
		this->foreach([&](const T& d) { if (!other.exists(d)) { ret.insert(d); } });
		return ret;
	}

	CCDK_FORCEINLINE this_type& operator&(this_type const& other)
		const { return intersect(other); }

	CCDK_FORCEINLINE this_type& operator|(this_type const& other) const {
		return unions(other);
	}
	CCDK_FORCEINLINE this_type operator-(this_type const& other) const {
		return minus(other);
	}
};


ccdk_namespace_ct_end