#pragma once
#include<ccdk/compile.h>
#include<ccdk/preprocessor/range_prefix.h>
#include<ccdk/preprocessor/range_call.h>
#include<ccdk/preprocessor/uint8_add_one.h>
#include<ccdk/preprocessor/uint8_sub_one.h>
#include<ccdk/mpl/mcontainer/mcontainer_fwd.h>
#include<ccdk/mpl/mcontainer/extensible_.h>

ccdk_namespace_mpl_start

/* get type at Index*/
template<uint32 Index, typename... Args>
struct arg_pack_at_and_erase_at;

/* insert T at Index */
template<uint32 Index, typename T, typename... Args>
struct arg_pack_insert_at;

/* merge */
template<typename Pack, typename... Args>
struct arg_pack_merge;

template<typename... Args2, typename... Args1>
struct arg_pack_merge<arg_pack<Args2...>, Args1...>
{
	typedef arg_pack<Args1..., Args2...> type;
};

/* erase T at */

/* iterator */
template<int32 Index, typename Pack>
struct arg_pack_iterator_
{
	typedef typename Pack::deref deref;
	typedef typename Pack::next  next;
};

template<>
struct arg_pack<>
{

	typedef arg_pack          type;
	typedef forward_category  category;
	typedef uint32_<0>        size;
	typedef true_             empty;
	typedef arg_pack          clear;

	/* iterator */
	typedef arg_pack          begin;
	typedef arg_pack          end;
	/* can't deref */
	
	/* can't pop_front */
	/* no front */
	/* no next */

	template<typename P> struct push_front { typedef arg_pack<P> type; };
	template<typename P> struct push_back { typedef arg_pack<P> type; };

	template<typename P> struct merge;
	template<typename... Args2> struct merge< arg_pack<Args2...>> { typedef arg_pack<Args2...> type;	};
};
		
template<typename T, typename... Args> 
struct arg_pack<T, Args...>
{
	typedef random_category      category;
	typedef arg_pack             type;
	/*iterator*/
	typedef arg_pack             begin;
	typedef arg_pack<>           end;
	typedef arg_pack<Args...>    next;
	typedef T                    deref;
	
	/* attribute  */
	typedef uint32_<sizeof...(Args)+1> size;
	typedef false_               empty;
	typedef arg_pack<>           clear;

	/* random at */
	template<int32 Index>              using at = typename arg_pack_at_and_delete_at< Index, T, Args...>::at;
	template<int32 Index>              using erase_at = typename arg_pack_at_and_delete_at< Index, T, Args...>::erase_at;
	template<int32 Index, typename T2> using insert_at = arg_pack_insert_at<Index, T2, T, Args...>;

	/* front operation */
	typedef arg_pack<Args...>    pop_front;
	typedef T                    front;
	template<typename T2> using  push_front = arg_pack<T2, T, Args...>;

	/* back operation */
	template<typename T2> using  push_back = arg_pack<T, Args..., T2>;
	using                        pop_back  = erase_at<size::value - 1>;
	template<typename T2> using  push_back = insert_at<size::value - 1, T2>;

	/* merge */
	template<typename P>  using  merge = arg_pack_merge< P, T, Args...>;
};

#define CCDK_ARG_PACK_INSTANCE_MAX       50
#define CCDK_ARG_PACK_INSTANCE_MAX_SUB1  49


/* get 0 */
template<typename T0, typename... Args> 
struct arg_pack_at_and_delete_at<0, T0, Args...>
{ 
	typedef T0                at;   
	typedef arg_pack<Args...> erase;
};

#define CCDK_ENUM_ARG_PACK_AT_AND_DELETE_AT(Index, _, ...)                                               \
template<CCDK_PP_RANGE_PREFIX(typename T, 0, Index,(,)), typename... Args>                               \
struct arg_pack_at_and_delete_at<Index, CCDK_PP_RANGE_PREFIX(T, 0, Index, (, )), Args...>                \
{                                                                                                        \
	typedef T ## Index at;                                                                               \
	typedef arg_pack<CCDK_PP_RANGE_PREFIX(T, 0, CCDK_PP_SUB1(Index), (, )), Args...> erase;              \
};  

/* get 1~50 */
CCDK_PP_RANGE_CALL(1, CCDK_ARG_PACK_INSTANCE_MAX, 1, CCDK_ENUM_ARG_PACK_AT_AND_DELETE_AT, CCDK_PP_EMPTY);

/* get 50~ unlimit , clip to 0~50 */
template<uint32 Index, CCDK_PP_RANGE_PREFIX(typename T, 0, CCDK_ARG_PACK_INSTANCE_MAX, (, )), typename... Args>
struct arg_pack_at_and_delete_at<Index, CCDK_PP_RANGE_PREFIX(T, 0, CCDK_ARG_PACK_INSTANCE_MAX, (, )), Args...>
{
	typedef typename arg_pack_at_and_delete_at<Index - CCDK_ARG_PACK_INSTANCE_MAX, Args...>::at    at;
	typedef arg_pack<CCDK_PP_RANGE_PREFIX(T, 0, CCDK_ARG_PACK_INSTANCE_MAX_SUB1, (, ))>            head;
	typedef typename arg_pack_at_and_delete_at<Index - CCDK_ARG_PACK_INSTANCE_MAX, Args...>::erase tail;
	typedef merge_t<head, tail>                                                                    erase;
};

#undef CCDK_ENUM_ARG_PACK_AT

/* insert 0 */
template<typename T, typename... Args>
struct arg_pack_insert_at<0, T, Args...> { typedef arg_pack<T,Args...> type; };

/* insert 1~50 */
#define CCDK_ENUM_ARG_PACK_INSERT(Index, _,...)                                                         \
template<typename T, CCDK_PP_RANGE_PREFIX(typename T, 0, Index, (, )), typename... Args>                \
struct arg_pack_insert_at<CCDK_PP_ADD1(Index), T, CCDK_PP_RANGE_PREFIX(T, 0, Index, (, )), Args...>     \
{                                                                                                       \
	typedef arg_pack< CCDK_PP_RANGE_PREFIX(T, 0, Index, (, )), T, Args...> type;                        \
};

/* insert 1~50 */
CCDK_PP_RANGE_CALL(0, CCDK_ARG_PACK_INSTANCE_MAX_SUB1, 1, CCDK_ENUM_ARG_PACK_INSERT, CCDK_PP_EMPTY);

/* get 50~ unlimit , clip to 0~50 */
template<uint32 Index, CCDK_PP_RANGE_PREFIX(typename T, 0, CCDK_ARG_PACK_INSTANCE_MAX_SUB1, (, )), typename... Args>
struct arg_pack_insert_at<Index, CCDK_PP_RANGE_PREFIX(T, 0, CCDK_ARG_PACK_INSTANCE_MAX_SUB1, (, )), Args...>
{
	typedef arg_pack< CCDK_PP_RANGE_PREFIX(T, 0, CCDK_ARG_PACK_INSTANCE_MAX_SUB1, (, ))>   head;
	typedef typename arg_pack_insert_at<Index - CCDK_ARG_PACK_INSTANCE_MAX, Args...>::type tail;
	typedef merge_t<head, tail>                                                            type;
};

#undef CCDK_ENUM_ARG_PACK_INSERT

ccdk_namespace_mpl_end