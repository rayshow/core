#pragma once

#include<Core/mpl/if.h>
#include<Core/mpl/logic_less.h>
#include<Core/mpl/meta_function/apply.h>
#include<Core/mpl/meta_function/identity.h>
#include<Core/mpl/container/vector_decl.h>
#include<Core/mpl/container/set_decl.h>

namespace Aurora3D
{
	namespace mpl
	{
		namespace detail
		{
			template<typename S, typename T, typename Result>
			struct SetInsertImpl:public 
				BoolDeriveIf<Result::found,
					VectorInsert<S,Result::pos,T>,
					Identity<S>>{};
		}

		template<typename S, typename T, typename CmpFn = AddApply<Less> >
		struct SetInsert:public detail::SetInsertImpl<S,T, SetSearch<S,T>>{};
	}
}