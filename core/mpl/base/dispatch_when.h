#pragma once

#include<core/preprocessor/seq_foreach_item.h>
#include<core/mpl/base/int_.h>
#include<core/mpl/base/add_.h>
#include<core/mpl/base/condi_derive.h>

namespace core
{
	namespace mpl
	{
		
			template<typename T> struct trait_fn_indexer :public int_<-1> {};

#define IS_FN_DECL( name, index ,... )                    \
			template<typename T> struct name;             \
			template<typename T> struct trait_fn_indexer< name<T> > :public int_<index> {};
			A3D_PP_FOREACH_ITEM(IS_FN_DECL, (is_void, is_integer, is_const, is_pointer))
#undef  IS_FN_DECL

			namespace detail
			{
				template<typename... Args>
				struct acc_index_if :public add_< condi_derive< Args, trait_fn_indexer<Args>, int_<0> >...> {};

				template<typename... Args>
				struct acc_index :public add_< trait_fn_indexer<Args>...> {};
			}
		//}

		template<typename T, typename... Args>
		using dispatch_ = typename detail::acc_index_if<T, Args...>::type;

		template<typename T,typename... Args>
		using when_ = typename detail::acc_index<T, Args...>::type;

		
	}
}