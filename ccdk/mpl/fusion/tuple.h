#pragma once

#include<ccdk/mpl/base/logic_.h>
#include<ccdk/mpl/base/type_.h>
#include<ccdk/mpl/mcontainer/make_indice.h>
#include<ccdk/mpl/type_traits/decay.h>
#include<ccdk/mpl/type_traits/is_same.h>
#include<ccdk/mpl/type_traits/has_inner_type.h>
#include<ccdk/mpl/util/forward.h>
#include<ccdk/mpl/util/move.h>
#include<ccdk/mpl/fusion/imap.h>
#include<ccdk/mpl/mpl_module.h>

ccdk_namespace_mpl_fs_start

/*
	all modify operation(like insert) will generate new type and old tuple elements will move to new tuple
*/
template<typename... Args>
class tuple 
{
public:

	constexpr static uint32 size = sizeof...(Args);
	typedef imap< make_indice<size>, Args... > value_type;
	template<typename...> friend class tuple;
private:
	value_type content;


	/*  push back args */
	template< uint32... indice, typename... Args1, typename... Args2 >
	CCDK_FORCEINLINE constexpr  tuple(indice_pack<indice...>, tuple<Args1...>&& tp, Args2&&... args2)
		: content{ util::move(tp.content.template at<indice>())..., util::forward<Args2>(args2)... } {}

	/*  push front args */
	template< uint32... indice, typename... Args1, typename... Args2 >
	CCDK_FORCEINLINE constexpr  tuple(int, indice_pack<indice...>, tuple<Args1...>&& tp, Args2&&... args2)
		: content{ util::forward<Args2>(args2)..., util::move(tp.content.template at<indice>())...  } {}


	/* erase or pop front / pop back / earase */
	template< uint32... indice, typename... Args2, uint32 len >
	CCDK_FORCEINLINE constexpr  tuple( indice_pack<indice...>, tuple<Args2...>&& tp, uint32_<len>) 
		: content{ util::move(tp.content.template at<indice>())... } { }

	/* replace or insert  */
	template< uint32... indice1, uint32... indice3, typename Tp, typename... Args2 >
	CCDK_FORCEINLINE constexpr  tuple( indice_pack<indice1...>, indice_pack<indice3...>, Tp&& tp, Args2&&... args2 )
		: content{ util::move(tp.content.template at<indice1>())..., util::forward<Args2>(args2)..., util::move(tp.content.template at<indice3>())... }{}

	/* pop back implements */
	template< uint32 len, typename... Args2 >
	CCDK_FORCEINLINE constexpr auto  _pop_back_impl(arg_pack<Args2...>)  { return tuple<Args2...>{  make_indice<size - len>{}, util::move(*this), uint32_<len>{} }; }

	/* pop front implements */
	template< uint32 len, typename ... Args1 >
	CCDK_FORCEINLINE constexpr auto  _pop_front_impl(arg_pack<Args1...>)  { return tuple<Args1...>{ make_indice_from<len, size>{}, util::move(*this), uint32_<len>{}  }; }

	/* erase implements */
	template< uint32 start, uint32 end, typename... Args1 >
	CCDK_FORCEINLINE constexpr auto  _erase_impl(arg_pack<Args1...>) { return tuple<Args1...>{ make_indice_ignore<size, start, end>{}, util::move(*this), uint32_<end - start>{} }; }

	/* replace implements */
	template< uint32 start, uint32 end, typename... Args1, typename... Args2, typename... Args3 >
	CCDK_FORCEINLINE constexpr auto  _replace_impl(arg_pack<Args1...>, arg_pack<Args3...>, Args2&&... args2)  
	{ return tuple< Args1..., Args2..., Args3...> {  make_indice<start>{}, make_indice_from< end, size>{}, util::move(*this), util::forward<Args2>(args2)...  }; }

	/* insert implements */
	template< uint32 pos, typename... Args1, typename... Args2, typename... Args3 >
	CCDK_FORCEINLINE constexpr auto  _insert_impl(arg_pack<Args1...>, arg_pack<Args3...>, Args2&&... args2)
	{return tuple< Args1..., Args2..., Args3...> { make_indice<pos>{}, make_indice_from<pos, size>{}, util::move(*this), util::forward<Args2>(args2)... }; }

public:
	CCDK_FORCEINLINE constexpr  tuple() : content{} {}


	/* forward args */
	template<typename... Args1, typename = check_t< and_< is_convertible<Args1,Args>...> >>
	CCDK_FORCEINLINE explicit constexpr tuple(Args1 &&... args1) : content( util::forward<Args1>(args1)...) {}

	/* copy */
	CCDK_FORCEINLINE constexpr  tuple(tuple const& other) : content{ other.content } {}

	/* move  */
	CCDK_FORCEINLINE constexpr  tuple(tuple&& other) : content{ util::move(other.content) } {}

	/* template copy */
	template<typename... Args1, typename = check_t< and_< is_convertible<Args1, Args>...> >>
	CCDK_FORCEINLINE constexpr  tuple(tuple<Args1...> const& other) : content{ other.content } {}

	/* template move */
	template<typename... Args1, typename = check_t< and_< is_convertible<Args1, Args>...> >>
	CCDK_FORCEINLINE constexpr  tuple(tuple<Args1...> && other) : content{ util::move(other.content) } {}

	/* merge two tuple */
	template< uint32... indice1, uint32... indice2, typename... Args1, typename... Args2 >
	CCDK_FORCEINLINE constexpr  tuple(indice_pack<indice1...>, indice_pack<indice2...>, tuple<Args1...>&& t1, tuple<Args2...>&& t2)
		: content{ util::move(t1.content.template at<indice1>())...,  util::move(t2.content.template at<indice2>())... } {}

	/* constexpr index */
	template<typename T, T index, typename = check_in_range<index,0,size> >
	CCDK_FORCEINLINE constexpr decltype(auto) operator[](compile_t<T, index>) { return content.template at<index>(); }
	
	/* constexpr index */
	template<typename T, T index, typename = check_in_range<index, 0, size> >
	CCDK_FORCEINLINE constexpr decltype(auto) operator[](compile_t<T, index>) const { return content.template at<index>(); }

	/* constexpr index */
	template<uint32 index, typename = check_in_range<index, 0, size> >
	CCDK_FORCEINLINE constexpr decltype(auto) at() const { return content.template at<index>(); }

	/* push back at least 1 element */
	template<typename... Args1, typename = check_gequal< sizeof...(Args1), 1> >
	CCDK_FORCEINLINE constexpr auto push_back(Args1&&... args1)  { return tuple< Args..., Args1...>{  make_indice<size>{}, util::move(*this), util::forward<Args1>(args1)... }; }

	/* push front at least 1 element */
	template<typename... Args1, typename = check_gequal< sizeof...(Args1), 1> >
	CCDK_FORCEINLINE constexpr auto push_front(Args1&&... args1) { return tuple< Args1..., Args... >{  0, make_indice<size>{}, util::move(*this), util::forward<Args1>(args1)... };  }

	/* pop back len items */
	template<uint32 len = 1, typename = check_in_range< len, 1, size> >
	CCDK_FORCEINLINE constexpr auto  pop_back()  { return _pop_back_impl<len>( args_head< size - len, Args... >{} ); }

	/* pop front len items */
	template<uint32 len = 1, typename = check_in_range< len, 1, size> >
	CCDK_FORCEINLINE constexpr auto  pop_front()  { return _pop_front_impl<len>( args_tail< len, Args... >{} ); }
		
	/* erase elements [start, end) */
	template< uint32 start,  uint32 end = start+ 1, typename = check_in_range2<start, end, 0, size> >
	CCDK_FORCEINLINE constexpr auto  erase() { return _erase_impl<start, end>( args_erase<start, end - start, Args...>{} ); }
	
	/* replace item in [start, end) */
	template< uint32 start, uint32 end, typename T, typename... Args1 >
	CCDK_FORCEINLINE constexpr auto  replace(T&& t, Args1&&... args1 )  
	{
		typedef split_args< start,  end-start, Args... > pack;
		return _replace_impl<start, end>( typename pack::head{},  typename pack::tail{}, util::forward<T>(t), util::forward<Args1>(args1)...);
	}

	/* insert items at start */
	template< uint32 pos, typename T, typename... Args1, typename = check_gequal<pos, 0> >
	CCDK_FORCEINLINE constexpr auto  insert(T&& t, Args1&&... args1) 
	{
		typedef split_args<pos, 0, Args... > pack;
		return _insert_impl< pos >( typename pack::head{}, typename pack::tail{}, util::forward<T>(t), util::forward<Args1>(args1)... );
	}
};

/* connect two tuple */
template<typename... Args1, typename... Args2 >
CCDK_FORCEINLINE constexpr auto operator+( tuple<Args1...>& lh, tuple<Args2...>& rh)  { return tuple< Args1..., Args2...>{  make_indice<lh.size>{}, make_indice<rh.size>{}, util::move(lh), util::move(rh)  }; }

template<typename... Args1, typename... Args2>
CCDK_FORCEINLINE constexpr auto tuple_item_equal(uint32_<0>, tuple<Args1...>& lh, tuple<Args2...>& rh) noexcept { return lh.template at<0>() == rh.template at<0>(); }

template<uint32 index, typename... Args1, typename... Args2>
CCDK_FORCEINLINE constexpr auto tuple_item_equal(uint32_<index>, tuple<Args1...>& lh, tuple<Args2...>& rh) noexcept { return lh.template at<index>() == rh.template at<index>() && tuple_item_equal(uint32_<index - 1>{}, lh, rh); }

/* equal, compatible 2 tuple */
template<typename... Args1, typename... Args2, typename = check_t< and_< is_compatible<Args1,Args2>...>>>
CCDK_FORCEINLINE constexpr auto operator==(tuple<Args1...>& lh, tuple<Args2...>& rh) noexcept { return lh.size == rh.size && tuple_item_equal(lh, rh); }

ccdk_namespace_mpl_fs_end