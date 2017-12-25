#pragma once
#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/type_traits/is_empty.h>
#include<ccdk/mpl/type_traits/is_final.h>
#include<ccdk/mpl/type_traits/decay.h>
#include<ccdk/mpl/util/forward.h>
#include<ccdk/mpl/util/move.h>


ccdk_namespace_mpl_fs_start


//empty class, only for construct and destruct semantics
template<typename First, typename Second, bool = is_empty_v<First> && !is_final_v<First> >
struct compress_pair final : public First
{
	typedef First           first_type;
	typedef decay_t<Second> second_type;

private:
	second_type second;

public:
	compress_pair() : First(), second{} {}

	compress_pair(const Second& second) : First(), second{ second } {}

	compress_pair(const first_type& inFirst, const second_type& inSecond)
		: First{ inFirst }, second{ inSecond } {}

	First& get_first() & { return *this; }
	const First& get_first() const& { return *this; }
	First get_first() && { return util::move(*this); }

	second_type& get_second() & { return second; }
	const second_type& get_second() const& { return second; }
	second_type get_second() && { return util::move(second); }

};


template<typename First, typename Second>
struct compress_pair<First, Second, false>
{
	typedef decay_t<First>  first_type;
	typedef decay_t<Second> second_type;

private:
	first_type  first;
	second_type second;

public:

	compress_pair() : first{}, second{} {}

	compress_pair(Second&& second) : first(), second{ util::forward<Second>(second) } {}

	compress_pair(const first_type& inFirst, const second_type& inSecond)
		: first{ inFirst }, second{ inSecond } {}

	First& get_first() & { return first; }
	const First& get_first() const& { return second; }
	First get_first() && { return util::move(second); }

	second_type& get_second() & { return second; }
	const second_type& get_second() const& { return second; }
	second_type get_second() && { return util::move(second); }
};


ccdk_namespace_mpl_fs_end