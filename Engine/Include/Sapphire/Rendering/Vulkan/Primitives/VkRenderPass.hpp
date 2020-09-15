// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_VK_RENDER_PASS_GUARD
#define SAPPHIRE_RENDERING_VK_RENDER_PASS_GUARD

#include <vulkan/vulkan.h>

namespace Sa
{
	class VkDevice;
	class VkSwapChain;

	class VkRenderPass
	{
		::VkRenderPass mHandle = VK_NULL_HANDLE;

	public:
		void Create(const VkDevice& _device, const VkSwapChain& _swapChain);
		void Destroy(const VkDevice& _device);

		operator ::VkRenderPass() const noexcept;
	};
}

#endif // GUARD
