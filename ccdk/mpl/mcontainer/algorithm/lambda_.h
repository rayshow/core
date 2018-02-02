#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/base/apply_.h>
#include<ccdk/mpl/base/placeholder.h>
#include<ccdk/mpl/base/derive_if.h>
#include<ccdk/mpl/type_traits/has_inner_type.h>
#include<ccdk/mpl/mcontainer/arg_pack.h>
#include<ccdk/mpl/mcontainer/iterator_.h>

ccdk_namespace_mpl_start

	namespace mpl_impl { template<int N, typename L0> struct lambda_n; }

	template<typename T>
	struct lambda_
	{
		template<typename N, typename... NArgs>
		struct apply : apply_< T, N, NArgs...> {};
	};

	template< CCDK_MPL_TFN1( TFn), typename T, typename ... Args1>
	struct lambda_< TFn<T, Args1...> >
	{
		static constexpr int N = 1 + sizeof...(Args1);
		typedef arg_pack<T, Args1 ...> pack_type;

		//T == ___ && N == 1 || Args1+T contain no ___
		static_assert(
			(is_aplaceholder<T>::value && N == 1) || 
			!containe_aplaceholder<T, Args1...>::value, 
			"___ no need extra template parameter");

		template<typename T2, typename... Args2>
		struct apply :
			derive_if< 
				is_aplaceholder<T>,
				tapply_< TFn, T2, Args2...>,
				apply_< mpl_impl::lambda_n<N, pack_type>, add_apply_<TFn>, T2, Args2...> >
		{};
	};

	template<typename Fn, typename ... Args>
	struct apply_lambda :public apply_< lambda_<Fn>, Args...> {};


	namespace detail
	{

		template<typename T, typename NIndex>
		struct lambda_1;

		/* Fn::type exists, return Fn::type */
		template<typename Fn, bool = has_inner_type_v<Fn>>
		struct select_type : public Fn {};

		/* Fn::type not exists, return Fn::wrapper_type */
		template<typename Fn>
		struct select_type<Fn, false>
		{
			typedef typename Fn::wrap_type type;
		};

		/* if   Fn::template apply<Args...>::type exists return it
		   else return Fn::template Apply<Args...>::wrapper_type, wrapper_type from apply_< OriginalFn >::tempalte Apply<prefix,...>
		*/
		template<typename Fn, typename... Args>
		struct select_apply_type : public select_type<apply_<Fn, Args...>> {};

		template<typename Fn, typename... Args>
		using select_apply_type_t = typename select_apply_type<Fn, Args...>::type;

		//N >=1
		template<int N, typename L0>
		struct lambda_n {};

		//N == 1
		template<typename CurrIt>
		struct lambda_n<1, CurrIt >
		{
			typedef int32_<0>        count0;
			typedef next_t<CurrIt>   next_it;
			typedef deref_t<CurrIt>  curr_type;
			typedef deref_t<next_it> next_type;

			typedef derive_if< is_nplaceholder<next_type>, next_<count0>, count0> count1;
			typedef lambda_1<next_it, count1> P1;

			template<typename Fn, typename T, typename... Args>
			struct apply :public select_apply_type< Fn, select_apply_type_t<P1, T, Args...>> {};
		};

		//TCount == 2
		template <typename N0>
		struct lambda_n<2, N0>
		{
			typedef int32_<0> count0;
			typedef typename N0::next N1;
			typedef typename N0::type T1;
			typedef typename N1::next N2;
			typedef typename N1::type T2;

			typedef derive_if<is_placeholder_<T1>, next_<count0>, count0> count1;
			typedef derive_if<is_placeholder_<T2>, next_<count1>, count0> count2;

			typedef lambda_1<T1, count1> P1;
			typedef lambda_1<T2, count2> P2;

			//Fn is from AddApply<Fn>
			template<typename Fn, typename T, typename... Args>
			struct Apply : public select_apply_type_t< Fn,
				select_apply_type_t< P1, T, Args...>,
				select_apply_type_t< P2, T, Args...>> {};
		};

//		//typedef DeriveIf<IsNPlaceholder<Tn>, Next<Cn-1>, Cn-1> Cn;
//#define MPL_TYPEDEF_DERIVE_IF_FORMAT(ForeIndex, Index, _ ) typedef DeriveIf<IsPlaceholder<T ## Index ##> , Next<C ## ForeIndex ## >, C ## ForeIndex ## > C ## Index ##;
//#define MPL_TYPEDEF_DERIVE_IF_DECL(Start, End) A3D_PP_RANGE_FORE_INDEX_CALL(Start,End, MPL_TYPEDEF_DERIVE_IF_FORMAT, _)
//
//		//typedef Lambda1<Tn, Cn> Pn;
//#define MPL_TYPEDEF_LAMBDA1_FORMAT(Index, _1, _2) typedef Lambda1<T ## Index, C ## Index> P ## Index;
//#define MPL_TYPEDEF_LAMBDA1_DECL(Start, End) A3D_PP_RANGE_INNER_CALL(Start, End, 1, MPL_TYPEDEF_LAMBDA1_FORMAT, _)
//
//		//typename TypeChooseCallApply< Pn, N, NArgsprefix,...>::type , (last no ,)
//#define MPL_TYPECHOOSECALLAPPLY_FORMAT(Index, Count, Split) typename TypeChooseCallApply< P ## Index, N, NArgsprefix,...>::type A3D_PP_INT_IF( Count, A3D_PP_UNPACK, A3D_PP_INGORE) ## (Split)
//#define MPL_TYPECHOOSECALLAPPLY_DECL(Start, End) A3D_PP_RANGE_INNER_CALL(Start, End, 1, MPL_TYPECHOOSECALLAPPLY_FORMAT, (,) )
//
//		//TCount == Index
//#define MPL_LAMBDAN_SPECIALIZATION_FORMAT(Index, _1, _2)  \
//		template <typename N0>                        \
//		struct LambdaN<Index, N0>                     \
//		{                                             \
//			typedef Int_<0> C0;                       \
//			A3D_PP_RANGE_CHAIN_DECLARE(typedef typename N, ::next, N, 1, Index, (;))  \
//			A3D_PP_RANGE_CHAIN_DECLARE(typedef typename N, ::type, T, 1, Index, (;))  \
//			MPL_TYPEDEF_DERIVE_IF_DECL(1, Index)      \
//			MPL_TYPEDEF_LAMBDA1_DECL(1,Index)         \
//			template<typename Fn, typename N, typenameprefix,... NArgs>                      \
//			struct Apply : public TypeChooseCallApply< Fn, MPL_TYPECHOOSECALLAPPLY_DECL(1,Index)> {};  \
//		};
//
//		A3D_PP_RANGE_CALL(3, A3D_PP_PLACEHOLDER_MAX, 1, MPL_LAMBDAN_SPECIALIZATION_FORMAT, _);
//
//#undef      MPL_LAMBDAN_SPECIALIZATION_FORMAT
//#undef      MPL_TYPECHOOSECALLAPPLY_DECL
//#undef      MPL_TYPECHOOSECALLAPPLY_FORMAT
//#undef      MPL_TYPEDEF_LAMBDA1_DECL
//#undef      MPL_TYPEDEF_LAMBDA1_FORMAT
//#undef      MPL_TYPEDEF_DERIVE_IF_DECL
//#undef      MPL_TYPEDEF_DERIVE_IF_FORMAT

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

		//_1 ~ _16
		template<int32 Index, typename NIndex>
		struct lambda_1< arg_<Index>, NIndex >
		{
			static_assert(is_placeholder_<arg_<Index>>::value, "index in arg<index> out of setting.");

			template<typename T, typename... Args>
			struct apply : arg_<Index>::template apply<T, Args...> {};
		};

		//__ -> _X
		template<typename NIndex>
		struct lambda_1< arg_<-1>, NIndex>
		{
			static_assert(is_placeholder_<arg_<NIndex::value>>::value, "index in arg<index> out of setting.");

			template<typename T, typename... Args>
			struct apply :public arg_<NIndex::value>::template apply<T, Args...> {};
		};

		//another template-function
		template< CCDK_MPL_TFN1(TFn), typename NIndex, typename T, typename... Args>
		struct lambda_1< TFn<T, Args...>, NIndex>
		{
			static constexpr int N = 1 + sizeof...(Args);
			typedef arg_pack<T, Args...> pack;

			//T == __ && TArgs ==empty || T TArgs contain no __
			static_assert((is_aplaceholder<T>::value && N == 1) || 
				!containe_aplaceholder<T, Args...>::value, 
				"if all-placeholder is exists, extra-template-parameter is forbidden.");

			template<typename T2, typename... Args2>
			struct apply :public
				derive_if<
					is_aplaceholder<T>,
						tapply_< TFn, T2, Args2...>,   //Avoid 1 Parameter Fn Error
					    apply_< lambda_n<N, pack>, add_apply_<TFn>, T2, Args2...> > {};
		};
	}//detail end
ccdk_namespace_mpl_end