// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Vulkan/System/VkRenderFrame.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa
{
	VkRenderFrame::VkRenderFrame(uint32 _index,
		VkImage _image,
		VkImageView _imageView,
		VkCommandBuffer _graphicsCommandBuffer) noexcept :
		index{ _index },
		image{ _image },
		imageView{ _imageView },
		graphicsCommandBuffer{ _graphicsCommandBuffer }
	{
	}
}

#endif
