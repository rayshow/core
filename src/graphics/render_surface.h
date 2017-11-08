#pragma once
#include<memory>
#include"gpu_object.h"
#include"graphics_def.h"

namespace Aurora3D
{
	class RenderSurface
	{
	private:
		std::weak_ptr< Texture > owner_;
		GPUObjectHandle rt_handler_{};
	public:
		RenderSurface(TexturePtr owner) :owner_{ owner }{}
	};
}