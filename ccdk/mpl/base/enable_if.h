#pragma once

namespace ccdk
{
	namespace mpl
	{
		//if true, get type, otherwise get compile error
		template<bool B, typename T> struct enable_if {  };
		template<typename T> struct enable_if<true,T> { typedef T type; };

		//if false, get type
		template<bool B, typename T> struct disable_if {};
		template<typename T> struct disable_if<false, T> { typedef T type; };


		template<bool B,typename T> 
		using enable_if_t = typename enable_if<B, T>::type;

		template<bool B, typename T>
		using disable_if_t = typename disable_if<B, T>::type;

		template<bool B>
		using check = typename enable_if<B, void>::type;

		template<typename T>
		using check_t = check< T::value >;

		template<uint32 start, uint32 end>
		using check_range = typename enable_if< (end>start && start>=0), void > ::type;

		template<uint32 index, uint32 max>
		using check_less = typename enable_if< ( index < max ), void > ::type;

		template<uint32 index, uint32 max>
		using check_lequal = typename enable_if< (index < max), void > ::type;

		template<uint32 index, uint32 min>
		using check_greater = typename enable_if< (index > min), void > ::type;

		template<uint32 index, uint32 min>
		using check_gequal = typename enable_if< (index >= min), void > ::type;

		template<uint32 index, uint32 min, uint32 max>
		using check_in_range = typename enable_if< (index >= min && index < max && min >= 0), void>::type;
		
		template<uint32 start, uint32 end, uint32 min, uint32 max>
		using check_in_range2 = typename enable_if< (start >= min && end <= max && end >= start && min>=0 ), void>::type;
	}
}