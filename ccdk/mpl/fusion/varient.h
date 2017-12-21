#pragma once

#include<typeinfo>

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/base/arg_pack_find_index.h>
#include<ccdk/mpl/base/make_indice.h>
#include<ccdk/mpl/type_traits/max_align_t.h>
#include<ccdk/mpl/type_traits/decay.h>
#include<ccdk/mpl/util/forward.h>
#include<ccdk/mpl/util/swap.h>
#include<ccdk/mpl/fusion/bad_cast_exception.h>

ccdk_namespace_mpl_fs_start

template<uint32 index, typename T>
struct varient_base
{
	CCDK_FORCEINLINE
	constexpr varient_base() noexcept {}

	//construct T
	CCDK_FORCEINLINE constexpr 
	varient_base( char* memory, const T& t, int inIndex)
	{
		new((void*)memory) T{ t };
	}

	CCDK_FORCEINLINE constexpr 
	const varient_base&  
	destruct(int inIndex,  char* memory ) const
	{
		if (inIndex == index)
		{
			//DebugValue("deconstruct of :", typeid(T).name() );
			reinterpret_cast<T*>(memory)->~T();
		}	
		return *this;
	}
};

template<uint32 index, typename T>
const varient_base<index, T>& get_base(const varient_base<index, T>& type)
{
	return type;
}

template<typename T, typename... Args>
struct varient_impl;

//max support 255 different type
template<uint32... indice, typename... Args>
struct varient_impl< indice_pack<indice...>, Args...>
	:public varient_base<indice, Args>...
{
	static constexpr uint32 L = sizeof...(Args);
	static constexpr uint32 Size = max_align<0, Args...>::MaxSize;
private:
	char   memory[Size];
	uint8  index;

	void destruct_old()
	{
		arg_dummy_fn(get_base<indice>(*this).destruct(index, memory)...);
	}

public:
	template<
		typename P,
		uint32 PIndex = arg_pack_find_index_v<P, Args...>
	>
	varient_impl(const P& p)
		:varient_base<PIndex, P>{ memory, p, 1 },
		index(PIndex)
	{ 
		static_assert(PIndex <= 255 , "type not found in bound Types");
	}

	template<
		typename P,
		uint32 PIndex = arg_pack_find_index_v<P, Args...>
	>
	P& to()
	{
		static_assert(PIndex <= 255, "type not found in bound Types");
		if (PIndex == index)
		{
			return *reinterpret_cast<P*>((char*)memory);
		}
		
		throw bad_cast_exception{ "bad any cast from P to T at ccdk::mpl::fusion::limit_any::to<T>()" };
	}

	template<
		typename P,
		uint32 PIndex = arg_pack_find_index_v<P, Args...>
	>
	varient_impl& operator=(const P& p)
	{
		static_assert(PIndex <= 255, "type not found in bound Types");
		destruct_old();
		new(memory) P{ p };
		index = PIndex;
		return *this;
	}
};

template<typename T, typename... Args>
using varient = varient_impl< make_indice< 1 + sizeof...(Args)>, T, Args...>;

ccdk_namespace_mpl_fs_end