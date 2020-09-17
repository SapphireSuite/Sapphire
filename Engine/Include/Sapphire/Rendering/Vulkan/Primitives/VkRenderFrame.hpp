// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_VK_RENDER_FRAME_GUARD
#define SAPPHIRE_RENDERING_VK_RENDER_FRAME_GUARD

#include <Rendering/Config.hpp>

#include <Rendering/Framework/Primitives/IRenderFrame.hpp>

#if SA_RENDERING_API == SA_VULKAN

#include <vulkan/vulkan.h>

namespace Sa
{
	class VkRenderFrame : public IRenderFrame
	{
	public:
		VkImage image;
		VkImageView imageView;

		VkFramebuffer frameBuffer;
		VkCommandBuffer graphicsCommandBuffer;

		VkSemaphore acquireSemaphores;
		VkSemaphore presentSemaphores;

		VkFence mainFence;

		VkRenderFrame(VkImage _image,
			VkImageView _imageView,
			VkFramebuffer _frameBuffer,
			VkCommandBuffer _graphicsCommandBuffer,
			VkSemaphore _acquireSemaphores,
			VkSemaphore _presentSemaphores,
			VkFence _mainFence) noexcept;
	};
}

#endif

#endif // GUARD
