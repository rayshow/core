#pragma once

#include<vector>

namespace Aurora3D
{
	//dxt1 =BC1 dxt3=BC2 dxt5=BC3 ATI1=BC4 ATI2,3DC = BC5
	//BC1 3-component color and alpha	Color(5:6:5), Alpha(1) or no alpha  FourCC="DXT1"
	//BC2 3-component color and alpha	Color(5:6:5), Alpha(4 direct alpha) FourCC="DXT2"
	//BC3 3-component color and alpha	Color(5:6:5), Alpha(3 lookup index) FOURCC="DXT3"
	//BC4 1-component color	One component(8)                                FOURCC="ATI1"
	//BC5 2-component color	Two components(8:8)                             FOURCC="ATI2"
	//BC6 HDR 3-16bit component no alpha
	//BC7 3-component color and alpha   Color(4-7 per component) alpha(0~8)
	//ETC1 3-component color no alpha

	//BC1 BC2 BC3 color part
	//[   color0  ] 16bits   color at two poles
	//[   color1  ]
	//[xx xx xx xx] 8bit  per 2bit is a interpolating value
	//[xx xx xx xx]
	//[xx xx xx xx]
	//[xx xx xx xx]

	//BC1 alpha 
	//if color0>color1  c2 = 0.66c0+0.33c1 c3 = 0.33c0+0.66c1
	//otherwise color1>color0 c2 = 0.5c0+0.5c1 c3 = black

	//BC2 alpha 
	//[xxxx xxxx xxxx xxxx] 16bit  per 4bit is a alpha
	//[xxxx xxxx xxxx xxxx]
	//[xxxx xxxx xxxx xxxx]
	//[xxxx xxxx xxxx xxxx]
	
	//BC3 alpha
	//[    alpha0   ] 1byte
	//[    alpha1   ]
	//[xxx xxx xxx xxx xxx xxx xxx xxx]  3bit table index
	//[xxx xxx xxx xxx xxx xxx xxx xxx]
	//if alpha0>alpha1 then, alpha2 = (6alpha0+alpha1)/7 prefix,... alpha7 = (alpha0+6alpha1)/7
	//if alpha1>alpha0 then, alpha2 = (4alpha0+alpha1)/5 prefix,... alpha6 = 0 alpha7 = 255

	//BC4 one component
	//[      r0      ] 1byte
	//[      r1      ] 
	//[xxx xxx xxx xxx xxx xxx xxx xxx] 3bit per index, 3bytes 
	//[xxx xxx xxx xxx xxx xxx xxx xxx]

	//BC5 two component
	//[      r0      ] 1byte
	//[      r1      ] 
	//[xxx xxx xxx xxx xxx xxx xxx xxx] 3bit per index, 3bytes 
	//[xxx xxx xxx xxx xxx xxx xxx xxx]
	//[      g0      ] 1byte
	//[      g1      ] 
	//[xxx xxx xxx xxx xxx xxx xxx xxx] 3bit per index, 3bytes 
	//[xxx xxx xxx xxx xxx xxx xxx xxx]

	//BC6H todo
	//BC7 todo
	//ETC1 todo
	//ETC2 todo
	//PVRTC todo iphone ipad

	enum class TextureFormat:unsigned char
	{
		None = 0,
		Rgba,        
		             //         0.0f ~ 1.0f              -1.0f-0.0f            HD  gamma correct
		Bc1,         //dx       DXGI_FORMAT_BC1_UNORM,                        DXGI_FORMAT_BC1_UNORM_SRGB
		Bc2,         //dx       DXGI_FORMAT_BC2_UNORM,                        DXGI_BC3_UNORM_SRGB
		Bc3,         //dx       DXGI_FORMAT_BC3_UNORM,                        DXGI_FORMAT_BC3_UNORM_SRGB
		Bc4,		 //dx       DXGI_FORMAT_BC4_UNORM,  DXGI_FORMAT_BC4_SNORM
		Bc5,         //dx       DXGI_FORMAT_BC5_UNORM,  DXGI_FORMAT_BC5_SNORM
		Bc6,         //dx       DXGI_FORMAT_BC6H_UF16   DXGI_FORMAT_BC6H_SF16
		Bc7,         //dx       DXGI_FORMAT_BC7_UNORM   DXGI_FORMAT_BC7_UNORM_SRGB
		Etc1,
		Etc2,
		Pvrtc_rgb_2bpp,
		Pvrtc_rgba_2bpp,
		Pvrtc_rgb_4bpp,
		Pvrtc_rgba_4bpp,
	};

	struct CompressedLevel
	{
		unsigned width = 0;
		unsigned height = 0;
		unsigned depth = 0;
		unsigned char* data_{};
		unsigned blockSize_{};
		unsigned dataSize_{};
		unsigned rowSize_{};
		unsigned rows_{};
		TextureFormat format_{};
		bool Decompress(unsigned char* dest);
	};

	class Image
	{
	private:
		int width_;
		int height_;
		int depth_;
		unsigned components_;
		unsigned numCompressedLevels_;
		bool cubemap_;
		bool array_;
		bool sRGB_;
		TextureFormat compressedFormat_{};
		std::vector<unsigned char> data_{};
	};

}