#pragma once

#include<Core/mpl/if.h>
#include<Core/mpl/null_.h>
#include<Core/mpl/short_inner_type_decl.h>
#include<Core/mpl/container/vector_decl.h>
#include<Core/mpl/container/set_decl.h>

namespace Aurora3D
{
	namespace mpl
	{
		//search front and back S2[0] in S1
		template<typename S1, typename S2, typename CmpFn>
		struct SetInsersectPart :public
		{
			//search S2::front in S1
			typedef SetSearch<S1, VectorFrontT<S2>, CmpFn> front; 
			
			//true for S2::front greater than S1::back, no intersect
			//false need test S2::back in S1
			static constexpr bool noIntersectFront = (FoundPosV(front) == LengthV(S1) && NotFoundV(first));
			typedef BoolDeriveIf< noIntersectFront, front, SetSearch<S1, VectorBackT<S2>, CmpFn>> back;
			static constexpr bool intersect = (FoundPosV(back) == FoundPosV(front)); // on same side
		};

		//check exists insersect and store range
		template<typename S1,typename S2,typename CmpFn>
		struct SetInsersectResult
		{
			typedef SetInsersectPart<S1, S2, CmpFn> first;
			typedef BoolDeriveIf< first::intersect, SetInsersectPart<S1, S2, CmpFn>, first> second;
			static constexpr bool existsIntersect = first::intersect;
			static constexpr int start = FoundPosV()
		};


		template<typename S1, typename S2, typename CmpFn>
		struct SetIntersect
		{

		};
		

	}
}
