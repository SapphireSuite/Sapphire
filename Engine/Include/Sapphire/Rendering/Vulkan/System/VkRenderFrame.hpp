// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_VK_RENDER_FRAME_GUARD
#define SAPPHIRE_RENDERING_VK_RENDER_FRAME_GUARD

#include <Rendering/Config.hpp>

#include <Rendering/Framework/System/IRenderFrame.hpp>

#include <Rendering/Vulkan/Buffer/VkUniformBuffer.hpp>

#if SA_RENDERING_API == SA_VULKAN

#include <vulkan/vulkan.h>

namespace Sa
{
	class VkRenderFrame : public IRenderFrame
	{
	public:
		uint32 index = 0u;

		VkImage image;
		VkImageView imageView;

		VkCommandBuffer graphicsCommandBuffer;

		VkRenderFrame(uint32 _index,
			VkImage _image,
			VkImageView _imageView,
			VkCommandBuffer _graphicsCommandBuffer) noexcept;
	};
}

#endif

#endif // GUARD
