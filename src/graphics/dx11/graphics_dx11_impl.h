#pragma once

#include<unordered_map>
#include<unordered_set>

#include<d3d11.h>
#include<dxgi.h>

#include"../graphics_def.h"
#include"../graphics.h"

#define DX_SAFE_RELEASE(p) if (p) { ((IUnknown*)p)->Release();  p = nullptr; }


namespace Aurora3D
{
	class GraphicsImpl
	{
	private:
		///context
		ID3D11Device* device_ = nullptr;
		ID3D11DeviceContext* context_ = nullptr ;
		IDXGISwapChain* swapchain_ = nullptr;

		///default back buffer
		ID3D11RenderTargetView* back_buffer_ = nullptr;
		ID3D11Texture2D* back_depth_tex_ = nullptr;
		ID3D11DepthStencilView* back_depth_stencil_ = nullptr;

		/// states
		std::unordered_map<unsigned, ID3D11BlendState*> blend_states_{};
		std::unordered_map<unsigned, ID3D11DepthStencilState*> depth_states_{};
		std::unordered_map<unsigned, ID3D11RasterizerState*> rasterizer_states_{};
		ID3D11SamplerState* samplers_[kMaxTextureUnits];

		///shader parameters
		bool rts_updated = true;
		ID3D11RenderTargetView* cur_render_targets[kMaxRenderSurface];
		ID3D11ShaderResourceView* shader_views_[kMaxTextureUnits];
		ID3D11Buffer* vertex_buffers_[kMaxVertexStream];
		ID3D11Buffer* const_buffers_[kMaxShaderType][kMaxParameterGroup];

		///helper
		std::unordered_set<unsigned> mutilsampleSet;
	public:
		ID3D11DeviceContext* GetDeviceContext() { return context_;  }
		ID3D11Device* GetDevice() { return device_;  }

		/// buffer operator
		HRESULT CreateEmptyBuffer(const D3D11_BUFFER_DESC& desc, ID3D11Buffer** buffer)
		{
			return device_->CreateBuffer(&desc, 0, buffer);
		}
		void CommitBufferData(ID3D11Buffer* buffer, void* data)
		{
			 context_->UpdateSubresource(buffer, 0, nullptr, data, 0, 0);
		}

		bool CreateGraphicsContext();

		bool InitGraphicsContext(void* window_handle,
			unsigned width, unsigned height, 
			unsigned multisamples, bool vsync, bool srgb);

		void CheckFeature(GraphicsFeature& feature);

		bool UpdateBackBuffer(unsigned width, unsigned height, unsigned multisamples);

		void ClearRendertarget(unsigned index,const float* color)
		{
			context_->ClearRenderTargetView(cur_render_targets[index], color);
		}
		void ClearDepthStencil(unsigned flag, float depth, unsigned stencil)
		{
			unsigned dx11_flag = 0;
			if (flag & ClearFlag::Depth) dx11_flag |= D3D11_CLEAR_DEPTH;
			if (flag & ClearFlag::Stencil) dx11_flag |= D3D11_CLEAR_STENCIL;
			context_->ClearDepthStencilView(back_depth_stencil_,
				dx11_flag, depth, stencil);
		}

		void Present()
		{
			swapchain_->Present(0, 0);
		}
	};

}