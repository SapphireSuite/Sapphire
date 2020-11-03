// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_VK_SWAP_CHAIN_GUARD
#define SAPPHIRE_RENDERING_VK_SWAP_CHAIN_GUARD

#include <vector>

#include <Core/Types/Int.hpp>

#include <Rendering/Image/ImageExtent.hpp>
#include <Rendering/Vulkan/System/VkRenderFrame.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa
{
	class VkDevice;
	class VkRenderSurface;
	class RenderPass;
	class VkQueueFamilyIndices;

	class VkSwapChain
	{
		struct Synchronisation
		{
			VkSemaphore acquireSemaphore	= VK_NULL_HANDLE;
			VkSemaphore presentSemaphore	= VK_NULL_HANDLE;
			VkFence		fence				= VK_NULL_HANDLE;
		};

		VkSwapchainKHR mHandle = VK_NULL_HANDLE;

		ImageExtent mExtent;
		VkFormat mImageFormat;

		uint32 mFrameIndex = 0u;
		uint32 mImageIndex = 0u;

		std::vector<vk::Framebuffer*> mFrames;
		std::vector<Synchronisation> mFramesSynchronisation;

		void CreateSwapChainKHR(const VkDevice& _device, const VkRenderSurface& _surface,
								const VkQueueFamilyIndices& _queueFamilyIndices);
		void DestroySwapChainKHR(const VkDevice& _device);

		void CreateFrames(const VkDevice& _device, const VkRenderSurface& _renderPass);
		void DestroyFrames(const VkDevice& _device);

		void CreateSynchronisation(const VkDevice& _device);
		void DestroySynchronisation(const VkDevice& _device);
	
	public:
		void AddRenderPass(const RenderPass* _renderPass);

		uint32 GetImageNum() const noexcept;
		VkFormat GetImageFormat() const noexcept;

		const ImageExtent& GetImageExtent() const noexcept;

		SA_ENGINE_API const VkRenderFrame GetRenderFrame() const noexcept;

		void Create(const VkDevice& _device, const VkRenderSurface& _surface, const VkQueueFamilyIndices& _queueFamilyIndices);
		void Destroy(const VkDevice& _device);

		// Optimized re-creation.
		void ReCreate(const VkDevice& _device, const VkRenderSurface& _surface, const VkQueueFamilyIndices& _queueFamilyIndices);

		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API VkRenderFrame Begin(const VkDevice& _device);
		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API void End(const VkDevice& _device);
	};
}

#endif

#endif // GUARD
