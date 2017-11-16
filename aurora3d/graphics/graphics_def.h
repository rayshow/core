#pragma once

#include"../precompile_def.h"
#include<memory>

namespace Aurora3D
{
	const unsigned int kMaxRenderSurface = 4;
	const unsigned int kMaxTextureUnits = 16;
	const unsigned int kMaxVertexStream = 4;
	const unsigned int kMaxShaderType = 5;
	const unsigned int kMaxParameterGroup = 5;

	class GPUObject;
	class RenderSurface;
	class Texture;
	class Texture2D;
	class TextureCubemap;
	class VertexBuffer;
	class ConstantBuffer;
	class IndexBuffer;
	class Graphics;
	class GraphicsImpl;
	class WindowImpl;
	class PlatformSupport;
	class ShaderInstance;
	class ShaderSource;
	class ShaderCompose;

	typedef std::shared_ptr<GPUObject> GPUObjectPtr;
	typedef std::shared_ptr<RenderSurface> RenderSurfacePtr;
	typedef std::shared_ptr<Texture> TexturePtr;
	typedef std::shared_ptr<Texture2D> Texture2DPtr;
	typedef std::shared_ptr<TextureCubemap> TextureCubemapPtr;
	typedef std::shared_ptr<VertexBuffer> VertexBufferPtr;
	typedef std::shared_ptr<ConstantBuffer> ConstBufferPtr;
	typedef std::shared_ptr<IndexBuffer> IndexBufferPtr;
	typedef std::shared_ptr<Graphics> GraphicsPtr;
	typedef std::shared_ptr<GraphicsImpl> GraphicsImplPtr;
	typedef std::shared_ptr<WindowImpl> WindowImplPtr;
	typedef std::shared_ptr<ShaderSource> ShaderSourcePtr;
	typedef std::shared_ptr<ShaderInstance> ShaderInstancePtr;
	typedef std::shared_ptr<ShaderCompose> ShaderComposePtr;

	enum class PrimitiveType :unsigned char
	{
		TriangleList,
		TriangleStrip,
		TriangleFan,
		LineList,
		LineStrip,
		PointList
	};

	enum class GeometryType:unsigned char 
	{
		Static = 0,
		Skinned,
		Instanced,
		Billboard
	};

	enum class ShaderType:unsigned char
	{
		Vertex,
		Pixel,
		Compute,
		Tessellation,
		Geometry
	};

	enum class ShaderParameterGroup:unsigned char
	{
		Frame = 0,
		Camara,
		Light,
		Material,
		Object
	};

	/// Texture filtering mode.
	enum class TextureFilterMode:unsigned char
	{
		Nearest = 0,
		Bilinear,
		Trilinear,
		Anisotropic,
		Default,
		Max
	};

	/// Texture addressing mode.
	enum class TextureAddressMode:unsigned char
	{
		Wrap = 0,
		Mirror,
		Clamp,
		Border,
		Max
	};

	/// Texture usage types.
	enum class TextureUsage:unsigned char
	{
		Static =0,
		Dynamic,
		Rendertarget,
		Depthstencil,
		Max
	};

	enum class LockState:unsigned char
	{
		Hardware = 0,
		Shadow,
		Scratch,
		Max
	};

	enum class BlendMode :unsigned char
	{
		Replace = 0,
		Add,
		Multiply,
		Alpha
	};

	enum class CompareMode :unsigned char
	{
		Always = 0
	};

	enum class CullMode :unsigned char
	{
		CullNone = 0
	};

	enum class FillMode :unsigned char
	{
		Solid = 0
	};

	enum class StencilOp :unsigned char
	{
		Keep = 0
	};

	enum class CubemapFace :unsigned char
	{
		Postive_X = 0,
		Negative_X,
		Postive_Y,
		Negative_Y,
		Postive_Z,
		Negative_Z
	};

	struct StencilCfg
	{
		CompareMode cmp = CompareMode::Always;
		StencilOp pass = StencilOp::Keep;
		StencilOp fail = StencilOp::Keep;
		StencilOp zfail = StencilOp::Keep;
		unsigned refer = 0;
		unsigned cmp_bit_mask = 0;
		unsigned write_bit_mask = 0;
	};

	struct ClearFlag
	{
		const static unsigned Color = 1u;
		const static unsigned Depth = 2u;
		const static unsigned Stencil = 4u;
		const static unsigned All = 7u;
	};
}