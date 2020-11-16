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

		Vec2ui mExtent;
		Format mFormat = Format::sRGBA_32;

		uint32 mImageNum = 1u;
		uint32 mFrameIndex = 0u;
		uint32 mImageIndex = 0u;

		struct Synchronisation
		{
			VkSemaphore acquireSemaphore = VK_NULL_HANDLE;
			VkSemaphore presentSemaphore = VK_NULL_HANDLE;
			VkFence		fence = VK_NULL_HANDLE;
		};

		std::vector<Synchronisation> mFramesSynch;

		std::vector<FrameBuffer*> mFrameBuffers;


		void CreateSwapChainKHR(const Device& _device, const RenderSurface& _surface);
		void DestroySwapChainKHR(const Device& _device);

		void CreateSynchronisation(const Device& _device);
		void DestroySynchronisation(const Device& _device);

	public:
		Format GetFormat() const noexcept;

		void Create(const Device& _device, const RenderSurface& _surface);
		void Destroy(const Device& _device);

		RenderFrame Begin(const Device& _device);
		void End(const Device& _device);

		const std::vector<FrameBuffer*>& CreateFrameBuffers(const Device& _device, const RenderPass& _renderPass,
			const RenderPassDescriptor& _renderPassDesc, uint32* _size = nullptr);
		void DestroyFrameBuffers(const Device& _device);
	};
}

#endif

#endif // GUARD
