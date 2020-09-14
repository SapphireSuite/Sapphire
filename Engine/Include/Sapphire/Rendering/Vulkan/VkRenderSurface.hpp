// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_VK_RENDER_SURFACE_GUARD
#define SAPPHIRE_RENDERING_VK_RENDER_SURFACE_GUARD

#include <vulkan/vulkan.h>

#include <Rendering/Vulkan/SwapChain/VkSwapChain.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa
{
	class VkRenderSurface
	{
		VkSurfaceKHR mHandle = VK_NULL_HANDLE;

		VkSwapChain mSwapChain;

	public:
		void InitHandle(VkSurfaceKHR _newHandle);
		void UnInitHandle();

		void Create(const VkDevice& _device, const VkQueueFamilyIndices& _queueFamilyIndices);
		void Destroy(const VkDevice& _device);

		operator VkSurfaceKHR() const;
	};
}

#endif

#endif // GUARD
