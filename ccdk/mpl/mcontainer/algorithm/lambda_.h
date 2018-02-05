#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/base/apply_.h>
#include<ccdk/mpl/base/placeholder.h>
#include<ccdk/mpl/base/derive_if.h>
#include<ccdk/mpl/type_traits/has_inner_type.h>
#include<ccdk/mpl/mcontainer/arg_pack.h>
#include<ccdk/mpl/mcontainer/val_pack.h>
#include<ccdk/mpl/mcontainer/iterator_.h>
#include<ccdk/mpl/mcontainer/algorithm/count_if_.h>

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

	template< CCDK_MPL_TFN1( TFn), typename T, typename ... Args>
	struct lambda_< TFn<T, Args...> >
	{
		static constexpr int N = 1 + sizeof...(Args);
		typedef arg_pack<T, Args ...> pack_type;
		typedef make_indice<N>         indice_type;

		//T == ___ && N == 1 || Args1+T contain no ___
		static_assert(
			(is_aplaceholder<T>::value && N == 1) || 
			!containe_aplaceholder<T, Args...>::value,
			"___ no need extra template parameter");

		template<typename T2, typename... Args2>
		struct apply :
			derive_if< 
				is_aplaceholder<T>,
				call_< TFn, T2, Args2...>,
				apply_< mpl_impl::lambda_n<indice_type, pack_type>, add_apply_<TFn>, T2, Args2...> >
		{};
	};

	template<typename Fn, typename ... Args>
	struct apply_lambda :public apply_< lambda_<Fn>, Args...> {};


	namespace mpl_impl
	{
		/* if   Fn::template apply<Args...>::type exists return it
		   else return Fn::template Apply<Args...>::this_type, this_type from apply_< OriginalFn >::tempalte Apply<prefix,...>
		*/
		template<typename Fn, typename... Args>
		struct select_apply_type : public select_type<apply_<Fn, Args...>> {};

		template<typename Fn, typename... Args>
		using select_apply_type_t = typename select_apply_type<Fn, Args...>::type;

		template<uint32... Indice, typename... Args>
		struct lambda_n<indice_pack<Indice...>, arg_pack<Args...>>
		{
			typedef arg_pack<Args...> Container;

			template<typename Fn, typename T2, typename... Args2>
			struct apply:apply_< Fn,
					apply_t<
					lambda_1<Args, count_if_< Container, is_placeholder_, Indice + 1>>,
					T2, Args2...>... > {};
		};

		//normal type
		template<typename T, typename NIndex>
		struct lambda_1
		{
			template<typename T2, typename... Args>
			struct apply
			{
				typedef T type;
			};
		};

		// select Index type from Args...
		template<int32 Index, typename NIndex>
		struct lambda_1< arg_<Index>, NIndex >
		{
			template<typename... Args>
			struct apply : arg_<Index>::template apply<Args...> {};
		};

		//__ -> _N, select N type from Args...
		template<typename NIndex>
		struct lambda_1< arg_<-1>, NIndex>
		{
			template<typename... Args>
			struct apply :public arg_<NIndex::value>::template apply<Args...> {};
		};

		//recursive replace
		template< CCDK_MPL_TFN1(TFn), typename NIndex, typename T, typename... Args>
		struct lambda_1< TFn<T, Args...>, NIndex>: lambda_<TFn<T,Args...>> {};

	}//detail end
ccdk_namespace_mpl_end