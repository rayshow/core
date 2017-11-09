#pragma once

#include<core/preprocessor/seq_foreach_item.h>
#include<core/mpl/base/int_.h>
#include<core/mpl/base/condi_derive.h>

namespace core
{
	namespace mpl
	{
		namespace detail
		{
			template<typename T> struct test_fn_indexer :int_<-1> {};

#define IS_FN_DECL( name, index ,... )                    \
			template<typename T> struct name;             \
			template<typename T> struct test_fn_indexer< name<T> > :int_<index> {};
			A3D_PP_FOREACH_ITEM(IS_FN_DECL, (is_void, is_integer, is_const, is_pointer))
#undef  IS_FN_DECL

			template<typename T1, typename T2> struct add:public int_<T1::value+T2::value> {};
	
			template<typename... Args>
			struct accumulate_if;

			template<typename T>
			struct accumulate_if<T> :public condi_derive< T, test_fn_indexer<T>, int_<0> > {};

			template<typename T, typename... Args>
			struct accumulate_if<T,Args...> :public add< accumulate_if<T>, accumulate_if<Args...> > {};
		}

		template<typename T, typename... Args>
		using dispatch_ = typename detail::accumulate_if<T, Args...>::type;
		

		template<typename T,typename... Args>
		using when_ = typename detail::accumulate_if<T, Args...>::type;

		
	}
}