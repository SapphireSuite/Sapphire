// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Framework/Misc/RenderFormat.hpp>

#include <Collections/Debug>

namespace Sa
{
#if SA_RENDERING_API == SA_VULKAN

	VkFormat API_GetRenderFormat(RenderFormat _format)
	{
		switch (_format.channelNum)
		{
			case 1:
				switch (_format.type)
				{
					case RenderFormatType::RGB:
						return VK_FORMAT_R8_UNORM;
					case RenderFormatType::sRGB:
						return VK_FORMAT_R8_SRGB;
					default:
						SA_LOG("TextureFormat not supported yet!", Warning, Rendering);
						return VK_FORMAT_R8_SRGB;
				}
			case 2:
				switch (_format.type)
				{
					case RenderFormatType::RGB:
						return VK_FORMAT_R8G8_UNORM;
					case RenderFormatType::sRGB:
						return VK_FORMAT_R8G8_SRGB;
					default:
						SA_LOG("TextureFormat not supported yet!", Warning, Rendering);
						return VK_FORMAT_R8G8_SRGB;
				}
			case 3:
				switch (_format.type)
				{
					case RenderFormatType::RGB:
						return VK_FORMAT_R8G8B8_UNORM;
					case RenderFormatType::sRGB:
						return VK_FORMAT_R8G8B8_SRGB;
					default:
						SA_LOG("TextureFormat not supported yet!", Warning, Rendering);
						return VK_FORMAT_R8G8B8_SRGB;
				}
			case 4:
				switch (_format.type)
				{
					case RenderFormatType::RGB:
						return VK_FORMAT_R8G8B8A8_UNORM;
					case RenderFormatType::sRGB:
						return VK_FORMAT_R8G8B8A8_SRGB;
					default:
						SA_LOG("TextureFormat not supported yet!", Warning, Rendering);
						return VK_FORMAT_R8G8B8A8_SRGB;
				}
			default:
				SA_LOG("Channel num not supported yet!", Warning, Rendering);
				return VK_FORMAT_R8G8B8A8_SRGB;
		}
	}

#endif
}