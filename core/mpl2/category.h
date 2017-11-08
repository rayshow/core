#pragma once
namespace Aurora3D
{
	namespace mpl
	{
		struct ForwardCategoryTag {};
		struct BidirectionalCategoryTag:public ForwardCategoryTag {};
		struct RandomCategoryTag :public BidirectionalCategoryTag {};

	}
}