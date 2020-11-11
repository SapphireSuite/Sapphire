// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_VK_SWAP_CHAIN_GUARD
#define SAPPHIRE_RENDERING_VK_SWAP_CHAIN_GUARD

#include <Rendering/Vulkan/Buffers/VkFrameBuffer.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa::Vk
{
	class Device;
	class RenderSurface;

	class RenderPass;

	class SwapChain
	{
		VkSwapchainKHR mHandle = VK_NULL_HANDLE;

		uint32 mImageNum = 1u;
		Vec2ui mExtent;

		RenderFormat mFormat = RenderFormat::sRGBA_32;

		std::vector<std::vector<FrameBuffer>> frameBufferImages;

		void CreateSwapChainKHR(const Device& _device, const RenderSurface& _surface);
		void DestroySwapChainKHR(const Device& _device);

	public:
		RenderFormat GetRenderFormat() const noexcept;

		void Create(const Device& _device, const RenderSurface& _surface);
		void Destroy(const Device& _device);

		void AddRenderPass(const Device& _device, const RenderPass& _renderPass, const RenderPassDescriptor& _rpDesc);
		void RemoveRenderPass(const Device& _device, const RenderPass& _renderPass);
	};
}

#endif

#endif // GUARD
