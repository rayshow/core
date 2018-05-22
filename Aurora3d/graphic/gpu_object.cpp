
#include"gpu_object.h"
#include"graphics.h"
#include"../engine/engine.h"

namespace Aurora3D
{
	GPUObject::GPUObject(GraphicsPtr graphics)
	{
		if (graphics)
		{
			graphics_ = graphics;
			graphics->AddGPUObject(shared_from_this());
		}
	}

	GPUObject::~GPUObject()
	{
		auto gs = graphics_.lock();
		if (gs)
			gs->RemoveGPUObject(shared_from_this());
	}

	void GPUObject::OnDeviceLost()
	{
	}

	void GPUObject::OnDeviceReset()
	{
	}

	void GPUObject::Release()
	{
	}

	void GPUObject::ClearDataLost()
	{
		is_lost_ = false;
	}


}
