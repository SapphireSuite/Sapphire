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
		VkSwapchainKHR mHandle = VK_NULL_HANDLE;

		ImageExtent mExtent;
		VkFormat mImageFormat;

		uint32 mFrameIndex = 0u;
		uint32 mImageIndex = 0u;

		std::vector<VkRenderFrame> mFrames;

		void CreateSwapChainKHR(const VkDevice& _device, const VkRenderSurface& _surface,
								const VkQueueFamilyIndices& _queueFamilyIndices);
		void DestroySwapChainKHR(const VkDevice& _device);

		void CreateImageView(const VkDevice& _device);
		void DestroyImageView(const VkDevice& _device);

		void CreateFrameBuffers(const VkDevice& _device, const RenderPass& _renderPass);
		void DestroyFrameBuffers(const VkDevice& _device);

		void CreateCommandBuffers(const VkDevice& _device);
		void DestroyCommandBuffers(const VkDevice& _device);

		void CreateSemaphores(const VkDevice& _device);
		void DestroySemaphores(const VkDevice& _device);

		void CreateFences(const VkDevice& _device);
		void DestroyFences(const VkDevice& _device);

	public:
		uint32 GetImageNum() const noexcept;
		VkFormat GetImageFormat() const noexcept;

		VkImageView GetImageView(uint32 _index) const noexcept;
		const ImageExtent& GetImageExtent() const noexcept;

		VkRenderFrame GetRenderFrame() const noexcept;

		void Create(const VkDevice& _device, const VkRenderSurface& _surface, 
					const VkQueueFamilyIndices& _queueFamilyIndices, const RenderPass& _renderPass);
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
