#pragma once

#include<typeinfo>

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/base/arg_pack_find_index.h>
#include<ccdk/mpl/type_traits/decay.h>
#include<ccdk/mpl/util/forward.h>
#include<ccdk/mpl/util/swap.h>
#include<ccdk/mpl/fusion/bad_any_cast.h>

ccdk_namespace_mpl_fs_start

template<uint32 index, typename T>
struct limit_any_base
{
	constexpr limit_any_base() {}

	//construct T
	constexpr limit_any_base( const char* memory, const T& t, int inIndex)
	{
		new((void*)memory) T{ t };
	}

	constexpr const limit_any_base&  destruct(int inIndex,  char* memory ) const
	{
		if (inIndex == index)
		{
			DebugValue("deconstruct of :", typeid(T).name() );
			reinterpret_cast<T*>(memory)->~T();
		}	
		return *this;
	}
};

template<uint32 index, typename T>
const limit_any_base<index, T>& get_base(const limit_any_base<index, T>& type)
{
	return type;
}

template<typename T, typename... Args>
struct limit_any_impl;


//max support 255 different type
template<uint32... indice, typename... Args>
struct limit_any_impl< indice_pack<indice...>, Args...>
	:public limit_any_base<indice, Args>...
{
	static constexpr uint32 L = sizeof...(Args);
	
private:
	char memory[12];
	uint8  index;   //current type index, 
public:
	template<
		typename P,
		uint32 PIndex = arg_pack_find_index_v<P, Args...>
	>
	limit_any_impl(const P& p)
		:limit_any_base<PIndex, P>{ memory, p, 1 },
		index(PIndex)
	{ 
		static_assert(PIndex <= 255 , "type not found in bound Types");
	}

	void destruct_old()
	{
		arg_dummy_fn( get_base<indice>(*this).destruct(index, memory)...);
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
		
		throw bad_any_cast{ "bad any cast from P to T at ccdk::mpl::fusion::limit_any::to<T>()" };
	}


	template<
		typename P,
		uint32 PIndex = arg_pack_find_index_v<P, Args...>
	>
	limit_any_impl& operator=(const P& p)
	{
		static_assert(PIndex <= 255, "type not found in bound Types");
		destruct_old();
		new(memory) P{ p };
		index = PIndex;
		return *this;
	}

};


ccdk_namespace_mpl_fs_end