#pragma once

#include<ccdk/type.h>
#include<ccdk/mpl/base/val_pack.h>


namespace ccdk
{
	namespace mpl
	{
		namespace base_detail
		{
			template<typename T, T start, T end, T... args>
			struct val_pack_create_impl 
				:val_pack_create_impl<T, start + 1, end, args..., start> {};

			template<typename T, T end, T... args>
			struct val_pack_create_impl<T, end, end, args...> { typedef val_pack< T, args...> type; };
		}

		template<typename T, T start, T end>
		struct val_pack_create :base_detail::val_pack_create_impl<T, start, end> {};

		template<typename T, T start, T end>
		using val_pack_create_t = typename val_pack_create<T, start, end>::type;

		
	}
}