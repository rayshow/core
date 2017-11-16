#pragma once
#include<unordered_map>
#include<string>
#include<vector>
#include<unordered_set>
#include<string>
#include<memory>

#include"../math/vector2.h"
#include"../math/vector3.h"
#include"../math/color4.h"
#include"../math/int_rect.h"
#include"graphics_def.h"

namespace Aurora3D
{
	struct GraphicsFeature
	{
		bool anisotropy_support = false;
		bool dxt_support = false;
		bool etc_support = false;
		bool pvrtc_support = false;
		bool hd_shadow_support = false;
		bool instance_support = false;
		std::unordered_set<unsigned> srgb_mutilsamples_support{};
		std::unordered_set<unsigned> mutilsamples_support{};
		bool srgb_read_support = false;
		bool srgb_rendersurface_support = false;
		bool defered_support = false;
		bool prepass_support = false;
	};

	class Graphics
	{
		using VertexBufferPtrVec = std::vector<VertexBufferPtr>;
	private:
		///api implements
		GraphicsImplPtr impl_ = nullptr;
		
		///platform specialized
		GraphicsFeature feature_;

		///render parameter
		unsigned  mutilsamples_ = 1;
		bool vsync_ = false;
		bool srgb_ = false;
		unsigned  anisotropy_lev_ = 0;
		
		///scene scale
		unsigned num_vertices_ = 0;
		unsigned num_primitives_ = 0;
		unsigned num_batches_ = 0;

		///graphics objs
		std::unordered_set<GPUObjectPtr> gpu_objects_{};
		TexturePtr textures_[kMaxTextureUnits]{};
		bool tex_updated = true;
		RenderSurfacePtr cur_surfaces_[kMaxRenderSurface]{};
		bool render_surface_updated = true;
		RenderSurfacePtr cur_depthstencil_ = nullptr;
		VertexBufferPtr  cur_vertexbuffers_[kMaxVertexStream]{};
		IndexBufferPtr   cur_indexbuffer_ = nullptr;
		ShaderInstancePtr cur_vs_ = nullptr;
		ShaderInstancePtr cur_ps_ = nullptr;

		///render state
		BlendMode blend_ = BlendMode::Add;
		bool blend_updated = true;
		bool enable_color_write_ = true;
		CompareMode depth_cmp = CompareMode::Always;
		bool enable_depth_write_ = true;
		StencilCfg stencil_cfg{};
		bool enable_stencil_test_ = true;
		bool depth_state_updated = true;
		FillMode fill_ = FillMode::Solid;
		CullMode cull_ = CullMode::CullNone;
		bool raster_state_updated = true;
		IntRect scissor_rect_{};
		bool enable_scissor_test_ = true;
		bool use_clip_plane_ = false;
		//Vector4 clip_place_{};


		///inner helper functions
		void CheckMutilSample();
	public:
		Graphics();

		///context attribute
		void SetVsync(bool vsync){ vsync_ = vsync; }
		void SetMutilSample(unsigned samples){ mutilsamples_ = samples;  }
		void SetSRGB(bool srgb){ srgb_ = srgb;  }

		///event handler, init context
		void OnWindowChanged(std::shared_ptr<void> data);
		void OnWindowCreated(std::shared_ptr<void> data);
	
		///render functions
		bool BeginFrame();
		void EndFrame();
		void ClearFullScreen(unsigned flag = ClearFlag::All, const LinearColor4& clear = LinearColor4{}, float depth = 1.0f, unsigned stencil = 0u);
		void ClearQuad();

		bool ResolveToTexture(Texture2DPtr dest, const IntRect& viewport);
		void SetVertexBuffer(VertexBufferPtr buffer);
		void SetVertexBuffers(const VertexBufferPtrVec& buffers, unsigned instance_offset = 0);
		void SetIndexBuffer(IndexBufferPtr buffer);
		void SetShader(ShaderInstancePtr vs, ShaderInstancePtr ps);
		void SetRenderSurface(unsigned index, RenderSurfacePtr surface);
		void SetRenderTexture(unsigned index, TexturePtr tex);
		void SetComputeShader(ShaderInstancePtr cs);

		///manage gpu object
		void AddGPUObject(GPUObjectPtr obj){ gpu_objects_.emplace(obj); }
		void RemoveGPUObject(GPUObjectPtr obj){ gpu_objects_.erase(obj); }
	};
}
