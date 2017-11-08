#include<algorithm>
#include<stdio.h>

#include"graphics.h"
#include"gpu_object.h"
#include"../io/logger.h"
#include"../common_def.h"
#include"../event/window_evt.h"

#ifdef AURORA3D_DX11
#include"dx11/graphics_dx11_impl.h"
#endif


namespace Aurora3D
{
	Graphics::Graphics()
	{
		impl_.reset(new GraphicsImpl{});
		if (!impl_)
		{
			A3D_FATAL("new graphics implements failed!");
			A3D_SEND_FATAL();
			return;
		}
		A3D_SUBSCRIBE_EVENT(WindowEvt::WindowCreated, &Graphics::OnWindowCreated);
		A3D_SUBSCRIBE_EVENT(WindowEvt::DisplayChanged, &Graphics::OnWindowChanged);
	}

	void Graphics::CheckMutilSample()
	{
		//check mutilsample support
		bool mutilsample_support = false;
		if (srgb_)
		{
			mutilsample_support = (1 == feature_.srgb_mutilsamples_support.count(mutilsamples_));
		}
		else{
			mutilsample_support = (1 == feature_.mutilsamples_support.count(mutilsamples_));
		}
		if (!mutilsample_support) mutilsamples_ = 1;
	}

	void Graphics::OnWindowCreated(EventData data)
	{
		auto disply_data = std::static_pointer_cast<WindowEvt::DisplayData>(data);
		
		//create device
		if (!impl_->CreateGraphicsContext())
		{
			A3D_FATAL("create graphics context failed!");
			A3D_SEND_FATAL();
			return;
		}
		//check feature
		impl_->CheckFeature(feature_);
		this->CheckMutilSample();

		//create swap chain
		if (!impl_->InitGraphicsContext(disply_data->window_handle,
			disply_data->width, disply_data->height, mutilsamples_, vsync_, srgb_))
		{
			A3D_FATAL("initialize graphics context failed!");
			A3D_SEND_FATAL();
			return;
		}

		this->ClearFullScreen();
		impl_->Present();

		A3D_INFO("context create with width:"
			+ std::to_string(disply_data->width) 
			+" height:" + std::to_string(disply_data->height)
			+" mutilsample:"+std::to_string(mutilsamples_)
			+ " vsync:" + std::to_string(vsync_)
			+ " srgb:" + std::to_string(srgb_));
	}

	
	void Graphics::OnWindowChanged(EventData data)
	{
		auto disply_data = std::static_pointer_cast<WindowEvt::DisplayData>(data);
		A3D_INFO( "on window changed with width:" + std::to_string(disply_data->width) + " height:" + std::to_string(disply_data->height));
	}


	void Graphics::ClearFullScreen(unsigned flag, const LinearColor4& c,
		float depth, unsigned stencil)
	{
		if (flag & ClearFlag::Color)
			impl_->ClearRendertarget(0, c.GetBuffer());
		if (flag & ClearFlag::Depth || flag& ClearFlag::Stencil)
			impl_->ClearDepthStencil(flag, depth, stencil);
	}
}