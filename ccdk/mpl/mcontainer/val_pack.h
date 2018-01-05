#pragma once

#include<ccdk/mpl/base/integer_.h>
#include<ccdk/mpl/mcontainer/mcontainer_fwd.h>

namespace ccdk
{
	namespace mpl
	{
		template<typename T>
		struct val_pack<T>
		{
			typedef forward_category     category;
			typedef val_pack             type;
			typedef val_pack             begin;
			typedef val_pack             end;
			
			template<T v>
			struct push_back
			{
				typedef val_pack<T, v> type;
			};

			template<typename T>
			struct merge;

			template<T... args2>
			struct merge< val_pack<T, args2...> >
			{
				typedef val_pack<T, args2...> type;
			};
		};

		template<typename T, T first, T... args>
		struct val_pack <T,first,args...>
		{ 
			typedef forward_category     category;
			typedef val_pack             type; 
			typedef val_pack             begin;
			typedef val_pack<T>          end;
			typedef val_pack<T, args...> next;
			typedef compile_t<T, first>  deref;
			typedef val_pack<T, args...> pop_front;
			
			template<T v>
			struct push_front
			{
				typedef val_pack<T, v, first, args...> type;
			};

			template<typename T>
			struct merge;

			template<T... args2>
			struct merge< val_pack<T, args2...> >
			{
				typedef val_pack<T, first, args..., args2...> type;
			};
		};

		template<uint32... indice> using indice_pack = val_pack<uint32, indice...>;

		template<typename T> struct is_indice_pack :false_ {};

		template<uint32... indice> struct is_indice_pack< indice_pack<indice...>> :true_ {};

		template<typename T, T c, T... args> struct val_first { static constexpr T value = c; };
	}
}