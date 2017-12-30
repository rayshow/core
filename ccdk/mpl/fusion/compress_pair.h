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

	second_type second;

	CCDK_FORCEINLINE compress_pair() : First(), second{} {}

	CCDK_FORCEINLINE compress_pair(const Second& second) : First(), second{ second } {}

	CCDK_FORCEINLINE compress_pair(const first_type& inFirst, const second_type& inSecond) : First{ inFirst }, second{ inSecond } {}

	/* first */
	CCDK_FORCEINLINE First&       get_first() noexcept       { return *this; }
	CCDK_FORCEINLINE const First& get_first() const noexcept { return *this; }

};


template<typename First, typename Second>
struct compress_pair<First, Second, false>
{
	typedef decay_t<First>  first_type;
	typedef decay_t<Second> second_type;

	first_type  first;
	second_type second;

	CCDK_FORCEINLINE compress_pair() : first{}, second{} {}

	CCDK_FORCEINLINE compress_pair(Second&& second) : first(), second{ util::forward<Second>(second) } {}

	CCDK_FORCEINLINE compress_pair(const first_type& inFirst, const second_type& inSecond) : first{ inFirst }, second{ inSecond } {}

	CCDK_FORCEINLINE First&	      get_first() noexcept       { return first; }
	CCDK_FORCEINLINE const First& get_first() const noexcept { return second; }
};


ccdk_namespace_mpl_fs_end