#include"graphics_dx11_impl.h"
#include"../../io/logger.h"
namespace Aurora3D
{
	bool GraphicsImpl::CreateGraphicsContext()
	{
		if ( nullptr == device_)
		{
			D3D11CreateDevice(nullptr,
				D3D_DRIVER_TYPE_HARDWARE,
				0, 0, nullptr, 0,
				D3D11_SDK_VERSION,
				&device_, nullptr,
				&context_);
		}
		if (!device_ || !context_)
		{
			A3D_ERROR("create grapphics device or context failed!");
			return false;
		}
		A3D_INFO("create dx11 device and context success!");
		return true;
	}


	bool GraphicsImpl::InitGraphicsContext(void* window_handle, 
		unsigned width, unsigned height,
		unsigned multisamples, bool vsync, bool srgb)
	{
		DXGI_SWAP_CHAIN_DESC swapchain_desc{};
		swapchain_desc.BufferCount = 1;
		swapchain_desc.BufferDesc.Width = width;
		swapchain_desc.BufferDesc.Height = height;
		swapchain_desc.BufferDesc.Format = srgb ? DXGI_FORMAT_R8G8B8A8_UNORM_SRGB :
			DXGI_FORMAT_R8G8B8A8_UNORM;

		swapchain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapchain_desc.OutputWindow = (HWND)window_handle;
		swapchain_desc.SampleDesc.Count = multisamples;
		swapchain_desc.SampleDesc.Quality = multisamples > 1 ? 0xffffffff : 0;
		swapchain_desc.Windowed = TRUE;
		swapchain_desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		IDXGIDevice* dxgi_device = nullptr;
		device_->QueryInterface(IID_IDXGIDevice, (void**)&dxgi_device);
		IDXGIAdapter* dxgi_adapter = nullptr;
		dxgi_device->GetParent(IID_IDXGIAdapter, (void**)&dxgi_adapter);
		IDXGIFactory* dxgi_factory = nullptr;
		dxgi_adapter->GetParent(IID_IDXGIFactory, (void**)&dxgi_factory);
		dxgi_factory->CreateSwapChain(device_, &swapchain_desc, &swapchain_);
		// After creating the swap chain, disable automatic Alt-Enter fullscreen/windowed switching
		// (the application will switch manually if it wants to)
		dxgi_factory->MakeWindowAssociation((HWND)window_handle, DXGI_MWA_NO_ALT_ENTER);

		dxgi_factory->Release();
		dxgi_adapter->Release();
		dxgi_device->Release();

		if (!swapchain_)
		{
			A3D_ERROR("create graphics swap chain failed!");
			return false;
		}
		A3D_INFO("create dx11 swap chain successed!");
		return true;
	}


	bool GraphicsImpl::UpdateBackBuffer(unsigned width, 
		unsigned height, unsigned multisamples)
	{
		ID3D11RenderTargetView *null_rt = nullptr;
		context_->OMSetRenderTargets(1, &null_rt, 0);
		DX_SAFE_RELEASE(back_buffer_);
		DX_SAFE_RELEASE(back_depth_stencil_);
		DX_SAFE_RELEASE(back_depth_tex_);
		memset(cur_render_targets, 0, kMaxRenderSurface*sizeof(ID3D11RenderTargetView*));
		rts_updated = true;
		swapchain_->ResizeBuffers(1, width, height, DXGI_FORMAT_UNKNOWN,
			DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);
		ID3D11Texture2D* back_buffer_tex;
		swapchain_->GetBuffer(0, IID_ID3D11Texture2D, (void**)&back_buffer_tex);
		if (!back_buffer_tex)
		{
			return false;
		}

		// create default depth-stencil texture and view
		D3D11_TEXTURE2D_DESC depthDesc;
		memset(&depthDesc, 0, sizeof depthDesc);
		depthDesc.Width = (UINT)width;
		depthDesc.Height = (UINT)height;
		depthDesc.MipLevels = 1;
		depthDesc.ArraySize = 1;
		depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthDesc.SampleDesc.Count = multisamples;
		depthDesc.SampleDesc.Quality = multisamples > 1 ? 0xffffffff : 0;
		depthDesc.Usage = D3D11_USAGE_DEFAULT;
		depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthDesc.CPUAccessFlags = 0;
		depthDesc.MiscFlags = 0;
		device_->CreateTexture2D(&depthDesc, 0, &back_depth_tex_);
		if (!back_buffer_) return false;
		device_->CreateDepthStencilView(back_depth_tex_, nullptr, &back_depth_stencil_);
		if (!back_depth_stencil_) return false;
		return true;
	}

	void GraphicsImpl::CheckFeature(GraphicsFeature& feature)
	{
		feature.anisotropy_support = true;
		feature.defered_support = true;
		feature.dxt_support = true;
		feature.etc_support = true;
		feature.pvrtc_support = true;
		feature.hd_shadow_support = true;
		feature.prepass_support = true;
		
		if (device_)
		{
			for (unsigned i = 0; i < 16; ++i)
			{
				unsigned quality = 0;
				device_->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, i, &quality);
				if (quality) feature.srgb_mutilsamples_support.emplace(i);
				quality = 0;
				device_->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, i,
					&quality);
				if (quality) feature.mutilsamples_support.emplace(i);
			}
		}
	}
}