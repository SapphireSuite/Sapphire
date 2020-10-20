// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_VK_RENDER_FRAME_GUARD
#define SAPPHIRE_RENDERING_VK_RENDER_FRAME_GUARD

#include <Rendering/Config.hpp>

#include <Rendering/Framework/System/IRenderFrame.hpp>

#include <Rendering/Vulkan/Buffer/VkUniformBuffer.hpp>

#include <Rendering/Vulkan/Buffer/VkCommandBuffer.hpp>

#if SA_RENDERING_API == SA_VULKAN

#include <vulkan/vulkan.h>

namespace Sa
{
	class VkRenderFrame : public IRenderFrame
	{
	public:
		uint32 index = 0u;

		CommandBuffer graphicsCommandBuffer;

		// Frame data
		VkImage image				= VK_NULL_HANDLE;
		VkImageView imageView		= VK_NULL_HANDLE;
		VkFramebuffer framebuffer	= VK_NULL_HANDLE;

		// Synchronisation
		VkSemaphore acquireSemaphore	= VK_NULL_HANDLE;
		VkSemaphore presentSemaphore	= VK_NULL_HANDLE;
		VkFence fence					= VK_NULL_HANDLE;
	};
}

#endif

#endif // GUARD
