#pragma once

#include<core/mpl/type_traits/has_operator_decl.h>

namespace Aurora3D
{
	namespace mpl
	{
		//for pre-unary operation declare
		namespace has_operation_detail
		{
			template<typename OpType>
			struct PostDecOperation
			{
				static constexpr auto Op()->decltype(Make<OpType>()++);
			};
			template<typename OpType>
			struct CheckPostDecParameter :public CheckParameterHelper<OpType>
			{
				static const bool value = 0;
			};
		}
		template<typename OpType, typename Ret = ingore_t,
			bool forbidden = has_operation_detail::CheckPostDecParameter<OpType>::value>
			struct HasPostDec :public has_operation_detail::HasUnaryOp<
			has_operation_detail::PostDecOperation<OpType>, OpType, Ret> {};

		template<typename OpType, typename Ret>
		struct HasPostDec<OpType, Ret, true> :False_ {};
	}
}