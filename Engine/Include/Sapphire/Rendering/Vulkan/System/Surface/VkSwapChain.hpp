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
		RenderFormat mFormat = RenderFormat::sRGBA_32;

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

		std::vector<uint32> mRenderPassIDs;
		std::vector<FrameBuffer> mFrameBuffers;


		void CreateSwapChainKHR(const Device& _device, const RenderSurface& _surface);
		void DestroySwapChainKHR(const Device& _device);

		void CreateSynchronisation(const Device& _device);
		void DestroySynchronisation(const Device& _device);

		void DestroyFrameBuffers(const Device& _device);

	public:
		RenderFormat GetRenderFormat() const noexcept;

		void Create(const Device& _device, const RenderSurface& _surface);
		void Destroy(const Device& _device);

		void Begin(const Device& _device);
		void End(const Device& _device);

		FrameInfos GetFrameInfos(uint32 _renderPassID);

		uint32 AddRenderPass(Device& _device, const RenderPass& _renderPass, const RenderPassDescriptor& _rpDesc);
		void RemoveRenderPass(Device& _device, uint32 _renderPassID);
	};
}

#endif

#endif // GUARD
