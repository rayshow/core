#pragma once

#include<Core/mpl/if.h>
#include<Core/mpl/choose.h>
#include<Core/mpl/logic_less.h>
#include<Core/mpl/logic_equal.h>
#include<Core/mpl/short_inner_type_decl.h>
#include<Core/mpl/meta_function/apply.h>
#include<Core/mpl/container/vector_decl.h>
#include<Core/mpl/container/set_decl.h>


namespace Aurora3D
{
	namespace mpl
	{
		template<typename S, int Pos, bool Found>
		struct SearchResult
		{
			typedef S base;
			static constexpr int  pos = Pos;
			static constexpr bool found = Found;
		};

		namespace detail
		{
			//binary search in [Begin, End)
			template<typename S, typename Key, typename Cmp,
				int Begin, int End, int Mid,
				typename Elem = VectorAtT<S, Mid>,
				bool Continue = GreaterV(End, Begin + 1)>
				struct SetSearchImpl : public DeriveChoose3<
					Equal<Key, Elem>,               //Key == Elem ??
					CallApply<Cmp, Key, Elem>,      //Key <  Elem ??
					SearchResult<S, Mid, true>,     //Key == Elem return Pos and found = true
					SetSearchImpl< S, Key, Fn, Begin, Mid, ShRightV(Begin + Mid, 1)>,    //Key < Elem  search front part
					SetSearchImpl< S, Key, Fn, Mid + 1, End, ShRightV(Mid + 1 + End, 1)>>  //Key > Elem  search back part
			{};

			//End == Mid+1
			template<typename S, typename Key, typename Cmp,
				int Begin, int End, int Mid, typename Elem>
				struct SetSearchImpl<S, Key, Cmp, Begin, End, Mid, Elem, false>:
					DeriveIf< Equal<Key, Elem>, 
						SearchResult<S, Mid, true>,
						SearchResult<S, Mid, false>>{};
		}

		template<typename S,typename Key,typename CmpFn = AddApply<Less>>
		struct SetSearch:public BoolDeriveIf<
			LengthV(S)!=0,
			detail::SetSearchImpl<S,Key,CmpFn,0,LengthV(S), ShRightV(LengthV(S),1)>, //S::length!=0
			SearchResult<S, 0, false>> //empty result Pos=0 and found = false
		{};
	}
}