// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_VK_SWAP_CHAIN_GUARD
#define SAPPHIRE_RENDERING_VK_SWAP_CHAIN_GUARD

#include <Rendering/Config.hpp>

#if SA_RENDERING_API == SA_VULKAN

#include <vulkan/vulkan.h>

namespace Sa::Vk
{
	class Device;
	class RenderSurface;

	class SwapChain
	{
		VkSwapchainKHR mHandle = VK_NULL_HANDLE;

	public:
		void Create(const Device& _device, const RenderSurface& _surface);
		void Destroy(const Device& _device);
	};
}

#endif

#endif // GUARD
