#pragma once
#include<ccdk/compile.h>
#include<ccdk/preprocessor/range_prefix.h>
#include<ccdk/preprocessor/range_call.h>
#include<ccdk/preprocessor/uint8_add_one.h>
#include<ccdk/preprocessor/uint8_sub_one.h>
#include<ccdk/mpl/mcontainer/mcontainer_fwd.h>

ccdk_namespace_mpl_start

namespace mpl_impl
{

#define CCDK_ARG_PACK_CLAMP_MAX       50
#define CCDK_ARG_PACK_CLAMP_MAX_SUB1  49
#define CCDK_NEED_CLAMP(N)            ((N)>CCDK_ARG_PACK_CLAMP_MAX)

	/* get type at Index*/
	template<int32 Index, bool Clamp , typename... Args> struct arg_pack_index_op;

	/* insert T at Index */
	template<int32 Index, bool Clamp, typename T, typename... Args> struct arg_pack_insert_at;

	/* merge */
	template<typename Pack, typename Pack2> struct arg_pack_merge;

	template< typename... Args1, typename... Args2>
	struct arg_pack_merge<arg_pack<Args1...>, arg_pack<Args2...>>
	{
		typedef arg_pack<Args1..., Args2...> type;
	};
}



template<>
struct arg_pack<>
{
	typedef random_category   category;
	typedef arg_pack          type;

	/* attribute */
	typedef uint32_<0>        size;
	typedef true_             empty;
	
	/* iterator, no deref, no next */
	typedef arg_pack          begin;
	typedef arg_pack          end;

	/*random , no at, no erase_at*/
	template<int32 Index, typename T> using insert_at = mpl_impl::arg_pack_insert_at<Index, CCDK_NEED_CLAMP(Index), T>;
	
	/* front operation, no pop_front, no front */
	template<typename P> using push_front = arg_pack<P>;

	/* back operation, no pop_back, no back */
	template<typename P> using push_back = arg_pack<P>;

	/* */
	typedef arg_pack            clear;
	template<typename P> using  merge = mpl_impl::arg_pack_merge< arg_pack, P>;
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
	typedef false_                     empty;
	
	/* random at */
	template<int32 Index>              struct at { static_assert(Index < size::value, "out of range");  typedef typename mpl_impl::arg_pack_index_op< Index, CCDK_NEED_CLAMP(Index), T, Args...>::at type; };
	template<int32 Index>              using erase_at = typename mpl_impl::arg_pack_index_op< Index, CCDK_NEED_CLAMP(Index), T, Args...>::erase_at;
	template<int32 Index, typename T2> using insert_at = typename mpl_impl::arg_pack_insert_at<Index, CCDK_NEED_CLAMP(Index), T2, T, Args...>::type;

	/* front operation */
	typedef arg_pack<Args...>    pop_front;
	typedef T                    front;
	template<typename T2> using  push_front = arg_pack<T2, T, Args...>;

	/* back operation */
	template<typename T2> using  push_back = arg_pack<T, Args..., T2>;
	using                        back      = typename at<size::value - 1>::type;
    using                        pop_back  = erase_at<size::value - 1>;

	/* container op */
	typedef arg_pack<>           clear;
	template<typename P>  using  merge = typename mpl_impl::arg_pack_merge< arg_pack, P >::type;
	template<int32 Index> using  split =  mpl_impl::arg_pack_index_op<Index - 1, CCDK_NEED_CLAMP(Index-1), T, Args...>;
};


template<int32 index, typename... Args>
struct args_split:arg_pack<Args...>::template split<index>
{
	typedef typename arg_pack<Args...>::template split<index> split_type;
	typedef typename split_type::split_head head;
	typedef typename split_type::split_tail tail;
};

template<int32 Index, typename... Args>
using args_head_t = typename args_split<Index, Args...>::head;

template<int32 Index, typename... Args>
using args_tail_t = typename args_split<Index, Args...>::tail;


namespace mpl_impl
{
	/* split offset-1 */
	template<typename... Args>
	struct arg_pack_index_op<-1, false, Args...>
	{
		typedef arg_pack<>         split_head;
		typedef arg_pack<Args...>  split_tail;
	};

	/* index_op 0 */
	template<typename T0, typename... Args> 
	struct arg_pack_index_op<0, false, T0, Args...>
	{ 
		typedef T0                   at;   
		typedef arg_pack<Args...>    erase_at;
		typedef arg_pack<T0>         split_head;
		typedef arg_pack<Args...>    split_tail;
	};

	#define CCDK_ENUM_ARG_PACK_INDEX_OP(Index, _, ...)                                                       \
	template<CCDK_PP_RANGE_PREFIX(typename T, 0, Index,(,)), typename... Args>                               \
	struct arg_pack_index_op<Index, false, CCDK_PP_RANGE_PREFIX(T, 0, Index, (, )), Args...>                 \
	{                                                                                                        \
		typedef T ## Index                                                               at;                 \
		typedef arg_pack<CCDK_PP_RANGE_PREFIX(T, 0, CCDK_PP_SUB1(Index), (, )), Args...> erase_at;           \
		typedef arg_pack<CCDK_PP_RANGE_PREFIX(T, 0, Index, (, ))>                        split_head;         \
		typedef arg_pack< Args...>                                                       split_tail;         \
	};  

	/* index_op 1~ CCDK_ARG_PACK_INSTANCE_MAX */
	CCDK_PP_RANGE_CALL(1, CCDK_ARG_PACK_CLAMP_MAX, 1, CCDK_ENUM_ARG_PACK_INDEX_OP, CCDK_PP_EMPTY);

	/* index_op CCDK_ARG_PACK_INSTANCE_MAX+1 ~ unlimit , clip to 0~ CCDK_ARG_PACK_INSTANCE_MAX */
	template<int32 Index, CCDK_PP_RANGE_PREFIX(typename T, 0, CCDK_ARG_PACK_CLAMP_MAX, (,)), typename... Args>
	struct arg_pack_index_op<Index, true, CCDK_PP_RANGE_PREFIX(T, 0, CCDK_ARG_PACK_CLAMP_MAX, (,)), Args...>
	{
		static constexpr int32 clamp_index = Index - 1 - CCDK_ARG_PACK_CLAMP_MAX;
		static constexpr bool  clamp_again = clamp_index > CCDK_ARG_PACK_CLAMP_MAX;

		typedef typename arg_pack_index_op<clamp_index, clamp_again, Args...>::at             at;
		typedef arg_pack<CCDK_PP_RANGE_PREFIX(T, 0, CCDK_ARG_PACK_CLAMP_MAX_SUB1, (, ))>      erase_front;
		typedef typename arg_pack_index_op<clamp_index, clamp_again, Args...>::erase_at       erase_tail;
		typedef typename arg_pack_merge<erase_front, erase_tail>::type                        erase_at;
		typedef arg_pack<CCDK_PP_RANGE_PREFIX(T, 0, CCDK_ARG_PACK_CLAMP_MAX, (, ))>           split_head_front;
		typedef typename arg_pack_index_op<clamp_index, clamp_again, Args...>::split_head     split_head_back;
		typedef typename arg_pack_index_op<clamp_index, clamp_again, Args...>::split_tail     split_tail;
		typedef typename arg_pack_merge<split_head_front, split_head_back>::type              split_head;
	};


	/* insert 0 */
	template<typename T, typename... Args>
	struct arg_pack_insert_at<0,false, T, Args...> { typedef arg_pack<T, Args...> type; };

	/* insert 1~50 */
	#define CCDK_ENUM_ARG_PACK_INSERT_AT(Index, _,...)                                                            \
	template<typename T, CCDK_PP_RANGE_PREFIX(typename T, 0, Index, (, )), typename... Args>                      \
	struct arg_pack_insert_at<CCDK_PP_ADD1(Index),false, T, CCDK_PP_RANGE_PREFIX(T, 0, Index, (, )), Args...>     \
	{                                                                                                             \
		typedef arg_pack< CCDK_PP_RANGE_PREFIX(T, 0, Index, (, )), T, Args...> type;                              \
	};

	/* insert 1~CCDK_ARG_PACK_INSTANCE_MAX-1 */
	CCDK_PP_RANGE_CALL(0, CCDK_ARG_PACK_CLAMP_MAX_SUB1, 1, CCDK_ENUM_ARG_PACK_INSERT_AT, CCDK_PP_EMPTY);

	/* get CCDK_ARG_PACK_INSTANCE_MAX ~ unlimit , clip to 0 ~ CCDK_ARG_PACK_INSTANCE_MAX-1 */
	template<int32 Index, typename T, CCDK_PP_RANGE_PREFIX(typename T, 0, CCDK_ARG_PACK_CLAMP_MAX_SUB1, (, )), typename... Args>
	struct arg_pack_insert_at<Index, true, T, CCDK_PP_RANGE_PREFIX(T, 0, CCDK_ARG_PACK_CLAMP_MAX_SUB1, (, )), Args...>
	{
		static constexpr int32 clamp_index = Index - CCDK_ARG_PACK_CLAMP_MAX;
		static constexpr bool  clamp_again = clamp_index > CCDK_ARG_PACK_CLAMP_MAX;

		typedef arg_pack< CCDK_PP_RANGE_PREFIX(T, 0, CCDK_ARG_PACK_CLAMP_MAX_SUB1, (, ))>   head;
		typedef typename arg_pack_insert_at<clamp_index, clamp_again, T,Args...>::type        tail;
		typedef typename arg_pack_merge<head, tail>::type                                   type;
	};

#undef CCDK_ENUM_ARG_PACK_INDEX_OP
#undef CCDK_ENUM_ARG_PACK_INSERT_AT
#undef CCDK_ARG_PACK_CLAMP_MAX
#undef CCDK_ARG_PACK_CLAMP_MAX_SUB1
#undef CCDK_NEED_CLAMP

}/* end of mpl_impl */

ccdk_namespace_mpl_end