#pragma once

#include<ccdk/mpl/util/addressof.h>
#include<ccdk/mpl/util/move.h>
#include<ccdk/mpl/iterator/ptr_iterator.h>
#include<ccdk/mpl/fusion/range.h>
#include<ccdk/string/string_module.h>
#include<ccdk/string/char_traits.h>

ccdk_namespace_string_start
using namespace mpl;

template<typename Char, typename Size = ptr::size_t>
class basic_string_view
{
public:
	typedef Char                     value_type;
	typedef Char                     char_type;
	typedef Char*                    pointer_type;
	typedef Char const*              const_pointer_type;
	typedef Char&                    reference_type;
	typedef Char const&              const_reference_type;
	typedef Size                     size_type;
	typedef basic_string<Char, Size> string_type;
	typedef char_traits<Char>        traits_type;
	typedef basic_string_view        this_type;
private:
	const_pointer_type pointer; /* will not change original string */
	range<Size>        range;
public:
	/* from c-string */
	CCDK_FORCEINLINE basic_string_view(Char const* str) :pointer(str), range{ 0, traits_type::length(str) } {}
	CCDK_FORCEINLINE basic_string_view(Char const* str, size_type len) :pointer(str), range{0, len } {}
	/* from basic_string */

	CCDK_FORCEINLINE basic_string_view()
};

ccdk_namespace_string_end