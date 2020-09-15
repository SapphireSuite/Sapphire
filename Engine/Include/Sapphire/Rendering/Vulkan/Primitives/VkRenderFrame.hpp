// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_VK_RENDER_FRAME_GUARD
#define SAPPHIRE_RENDERING_VK_RENDER_FRAME_GUARD

#include <vulkan/vulkan.h>

#include <Rendering/Config.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa
{
	class VkRenderFrame
	{
	public:
		VkImage image;
		VkImageView imageView;

		VkFramebuffer frameBuffer;
		VkCommandBuffer graphicsCommandBuffer;

		VkSemaphore acquireSemaphores;
		VkSemaphore presentSemaphores;

		VkFence mainFence;
	};
}

#endif

#endif // GUARD
