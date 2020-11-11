// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Framework/Misc/RenderFormat.hpp>

#include <Collections/Debug>

namespace Sa
{
	bool IsColorFormat(RenderFormat _format) noexcept
	{
		return static_cast<uint8>(_format) >= static_cast<uint8>(RenderFormat::R_8) && static_cast<uint8>(_format) <= static_cast<uint8>(RenderFormat::sBGRA_32);
	}
	
	bool IsDepthFormat(RenderFormat _format) noexcept
	{
		return static_cast<uint8>(_format) >= static_cast<uint8>(RenderFormat::Stencil_8) && static_cast<uint8>(_format) <= static_cast<uint8>(RenderFormat::DepthStencil_32);
	}

#if SA_RENDERING_API == SA_VULKAN

	VkFormat API_GetRenderFormat(RenderFormat _format)
	{
		switch (_format)
		{
			// RGB.
			case RenderFormat::R_8:
				return VK_FORMAT_R8_UNORM;
			case RenderFormat::RG_16:
				return VK_FORMAT_R8G8_UNORM;
			case RenderFormat::RGB_32:
				return VK_FORMAT_A2R10G10B10_UNORM_PACK32;
			case RenderFormat::BGR_32:
				return VK_FORMAT_A2B10G10R10_UNORM_PACK32;
			case RenderFormat::RGBA_32:
				return VK_FORMAT_R8G8B8A8_UNORM;
			case RenderFormat::BGRA_32:
				return VK_FORMAT_B8G8R8A8_UNORM;

			// sRGB.
			case RenderFormat::sR_8:
				return VK_FORMAT_R8_SRGB;
			case RenderFormat::sRG_16:
				return VK_FORMAT_R8G8_SRGB;
			case RenderFormat::sRGBA_32:
				return VK_FORMAT_R8G8B8A8_SRGB;
			case RenderFormat::sBGRA_32:
				return VK_FORMAT_B8G8R8A8_SRGB;

			// Depth
			case RenderFormat::Stencil_8:
				return VK_FORMAT_S8_UINT;
			case RenderFormat::Depth_16:
				return VK_FORMAT_D16_UNORM;
			case RenderFormat::Depth_32:
				return VK_FORMAT_D32_SFLOAT;
			case RenderFormat::DepthStencil_32:
				return VK_FORMAT_D24_UNORM_S8_UINT;

			default:
				SA_LOG("RenderFormat not supported yet!", Warning, Rendering);
				return VK_FORMAT_R8G8B8A8_UNORM;
		}
	}

	RenderFormat API_FromRenderFormat(VkFormat _format)
	{
		switch (_format)
		{
			// RGB.
			case VK_FORMAT_R8_UNORM:
				return RenderFormat::R_8;
			case VK_FORMAT_R8G8_UNORM:
				return RenderFormat::RG_16;
			case VK_FORMAT_A2R10G10B10_UNORM_PACK32:
				return RenderFormat::RGB_32;
			case VK_FORMAT_A2B10G10R10_UNORM_PACK32:
				return RenderFormat::BGR_32;
			case VK_FORMAT_R8G8B8A8_UNORM:
				return RenderFormat::RGBA_32;
			case VK_FORMAT_B8G8R8A8_UNORM:
				return RenderFormat::BGRA_32;

			// sRGB.
			case VK_FORMAT_R8_SRGB:
				return RenderFormat::sR_8;
			case VK_FORMAT_R8G8_SRGB:
				return RenderFormat::sRG_16;
			case VK_FORMAT_R8G8B8A8_SRGB:
				return RenderFormat::sRGBA_32;
			case VK_FORMAT_B8G8R8A8_SRGB:
				return RenderFormat::sBGRA_32;

			// Depth
			case VK_FORMAT_S8_UINT:
				return RenderFormat::Stencil_8;
			case VK_FORMAT_D16_UNORM:
				return RenderFormat::Depth_16;
			case VK_FORMAT_D32_SFLOAT:
				return RenderFormat::Depth_32;
			case VK_FORMAT_D24_UNORM_S8_UINT:
				return RenderFormat::DepthStencil_32;

			default:
				SA_LOG("RenderFormat not supported yet!", Warning, Rendering);
				return RenderFormat::RGBA_32;
		}
	}

#endif
}