#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/base/apply_.h>
#include<ccdk/mpl/base/placeholder.h>
#include<ccdk/mpl/base/derive_if.h>
#include<ccdk/mpl/type_traits/has_inner_type.h>
#include<ccdk/mpl/mcontainer/arg_pack.h>
#include<ccdk/mpl/mcontainer/val_pack.h>
#include<ccdk/mpl/mcontainer/iterator_.h>

ccdk_namespace_mpl_start

	namespace mpl_impl { 
		template<typename Indice, typename Pack> struct lambda_n;
		template<typename T, typename NIndex> struct lambda_1;
	}

	template<typename T>
	struct lambda_
	{
		template<typename N, typename... NArgs>
		struct apply : apply_< T, N, NArgs...> {};
	};

	/* avoid double nest lambda_ */
	template<typename... Args>
	struct lambda_< lambda_<Args...>>:lambda_<Args...> {};

	template< CCDK_TFN1(TFn), typename ... Args>
	struct lambda_< TFn<Args...> >
	{
		static constexpr int N = sizeof...(Args);
		typedef arg_pack<Args ...>         pack_type;
		typedef make_indice_from<1, N+1>   indice_type;

		template<typename T2, typename... Args2>
		struct apply :mpl_impl::lambda_n<indice_type, pack_type>::template apply<add_apply_<TFn>, T2, Args2...>
		{};
	};

	template<typename T>
	using lambda_t = typename lambda_<T>::type;

	template<typename Fn, typename T, typename ... Args>
	struct apply_lambda_ :public apply_< lambda_<Fn>, T, Args...> {};

	namespace mpl_impl
	{
		
		template< typename It, typename Count, uint32 Step>
		struct count_placeholder_impl :
				  count_placeholder_impl< next_t<It>,
					derive_if< is_placeholder_<deref_t<It>>, next_t<Count>, Count>, Step - 1>
		{};

		template< typename It, typename Count>
		struct count_placeholder_impl<It, Count, 0> :Count {};
		
		/* count placeholder in Container */
		template<typename Container, uint32 Step>
		struct count_placeholder :count_placeholder_impl<begin_t<Container>, uint32_<0>, Step> {};

		/* if   Fn::template apply<Args...>::type exists return it
		   else return Fn::template Apply<Args...>::this_type, this_type from apply_< OriginalFn >::tempalte Apply<prefix,...>
		*/
		template<typename Fn, typename... Args>
		struct select_apply_type : public select_type_<apply_<Fn, Args...>> {};

		template<typename Fn, typename... Args>
		using select_apply_type_t = typename select_apply_type<Fn, Args...>::type;

		template<uint32... Indice, typename... Args>
		struct lambda_n<indice_pack<Indice...>, arg_pack<Args...>>
		{
			typedef arg_pack<Args...> Container;

			template<typename Fn, typename T2, typename... Args2>
			struct apply:apply_< Fn,
					apply_t<
						lambda_1<Args, count_placeholder< Container, Indice>>,
						T2, Args2...
						>... 
			> {};
		};

		//normal type, keep unchange
		template<typename T, typename NIndex>
		struct lambda_1
		{
			template<typename... Args>
			struct apply
			{
				typedef T type;
			};
		};

		// mapping placeholder to Type
		template<int64 Index, typename NIndex>
		struct lambda_1< arg_<Index>, NIndex >
		{
			template<typename... Args>
			struct apply
			{
				typedef apply_t< arg_< (Index > 0) ? Index : NIndex::value >, Args...> type;
			};
		};

		// is template, recursive replace placeholder
		template< CCDK_TFN1(TFn), typename NIndex, typename T, typename... Args>
		struct lambda_1< TFn<T, Args...>, NIndex>: lambda_<TFn<T,Args...>> {};

	}//detail end
ccdk_namespace_mpl_end