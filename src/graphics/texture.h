#pragma once

#include<vector>
#include"gpu_object.h"
#include"graphics_def.h"

namespace Aurora3D
{
	class Texture:public GPUObject
	{
	private:
		unsigned width_ = 0u;
		unsigned height_ = 0u;
		unsigned depth_ = 0u;
		GPUObjectHandle sampler_{};
		GPUObjectHandle input_tex_{};
		TextureUsage usage_ = TextureUsage::Static;
		std::vector<RenderSurfacePtr> rendertargets_;
	public:
		Texture(GraphicsPtr ptr):GPUObject(ptr){}
		virtual ~Texture(){}
	};
}