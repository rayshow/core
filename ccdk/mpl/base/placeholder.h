#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/preprocessor/range_call.h>
#include<ccdk/preprocessor/range_prefix.h>
#include<ccdk/preprocessor/sign.h>
#include<ccdk/mpl/base/type_.h>
#include<ccdk/mpl/base/logic_.h>

ccdk_namespace_mpl_start

template<int64 T> struct arg_ {};

typedef arg_<-1> __;    /* mapping single type according to position, _ may conflict with ph::_ */
typedef arg_<-2> ___;   /* mapping all input args */

/* is placeholder ? */
template<typename T> struct is_placeholder_ :public false_ {};
template<int32 N> struct is_placeholder_<arg_<N>> :public true_ {};

/* is _ ? */
template<typename T> struct is_nplaceholder :public false_ {};
template<> struct is_nplaceholder<arg_<-1>> :public true_ {};

/* is __ ? */
template<typename T> struct is_aplaceholder :public false_ {};
template<> struct is_aplaceholder<arg_<-2>> : public true_ {};

/* arg pack contain Arg<-1> ? */
template<typename... Args> 
struct contain_nplaceholder :public or_< is_nplaceholder<Args>...> {};

/* arg pack contain Arg<-2> ? */
template<typename... Args> 
struct containe_aplaceholder :public or_< is_aplaceholder<Args>... > {};

#define CCDK_PLACEHOLDER_SPECIALIZATION_DECL(N, index, ...)                                        \
template<> struct arg_<N>                                                                          \
{ template<CCDK_PP_RANGE_PREFIX(typename T, 1, N, (, ))> struct apply { typedef T ## N type; }; }; \
typedef arg_<N> _ ## N;

CCDK_PP_RANGE_CALL(1, 16, 1, CCDK_PLACEHOLDER_SPECIALIZATION_DECL, CCDK_PP_EMPTY);

ccdk_namespace_mpl_end