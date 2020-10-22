// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Framework/Primitives/Texture/TextureChannel.hpp>

#include <Collections/Debug>

namespace Sa
{
#if SA_RENDERING_API == SA_VULKAN

	VkFormat API_GetFormat(TextureChannel _channel)
	{
		switch (_channel)
		{
			case Sa::TextureChannel::Grey:
				return VK_FORMAT_R8_SRGB;
			case Sa::TextureChannel::GreyA:
				return VK_FORMAT_R8G8_SRGB;
			case Sa::TextureChannel::RGB:
			case Sa::TextureChannel::RGBA:
				return VK_FORMAT_R8G8B8A8_SRGB;
			default:
				SA_LOG("TextureChannel not supported yet!", Warning, Rendering)
				return VK_FORMAT_R8G8B8A8_SRGB;
		}
	}

#endif
}