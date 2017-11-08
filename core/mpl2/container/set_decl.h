#pragma once

#include<Core/mpl/logic_less.h>
#include<Core/mpl/meta_function/apply.h>

namespace Aurora3D
{
	namespace mpl
	{
		//set declare
		template<typename... Args> struct Set_;

#define FoundV(S)    (S::found)
#define NotFoundV(S) (!S::found)
#define FoundPosV(S) (S::pos)

		template<typename S, int Pos = S::length, bool Found = false>
		struct SearchResult;

		//return SearchResult which inner contain S as base
		//int Pos as indice, bool found as key found or not  
		template<typename S, typename Key, typename CmpFn = AddApply<Less>>
		struct SetSearch;

		//If   Key exists result S
		//else return new S inserted with Key
		template<typename S, typename Key, typename CmpFn = AddApply<Less> >
		struct SetInsert;

		//return wraped judge
		template<typename S, typename Key, typename CmpFn= AddApply<Less>>
		struct SetHasKey;
		
	}
}
