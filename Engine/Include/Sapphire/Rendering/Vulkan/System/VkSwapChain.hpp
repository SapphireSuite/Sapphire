// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_VK_SWAP_CHAIN_GUARD
#define SAPPHIRE_RENDERING_VK_SWAP_CHAIN_GUARD

#include <Rendering/APIConfig.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa::Vk
{
	class Device;
	class RenderSurface;

	class SwapChain
	{
		VkSwapchainKHR mHandle = VK_NULL_HANDLE;

		void CreateSwapChainKHR(const Device& _device, const RenderSurface& _surface);
		void DestroySwapChainKHR(const Device& _device);

	public:
		void Create(const Device& _device, const RenderSurface& _surface);
		void Destroy(const Device& _device);
	};
}

#endif

#endif // GUARD
