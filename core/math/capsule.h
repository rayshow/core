#pragma once

#include"../type.h"
#include"vector3.h"

namespace Aurora3D
{
	struct Capsule
	{
	public:
		Vector3 center;
		float   redius;
		Vector3 orientation;
		float   length;

		Capsule(){}

	};
}