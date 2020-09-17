// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_VK_SWAP_CHAIN_GUARD
#define SAPPHIRE_RENDERING_VK_SWAP_CHAIN_GUARD

#include <vector>

#include <Core/Types/Int.hpp>

#include <Rendering/Image/ImageExtent.hpp>
#include <Rendering/Vulkan/Primitives/VkRenderFrame.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa
{
	class VkDevice;
	class VkRenderSurface;
	class VkRenderPass;
	class VkQueueFamilyIndices;

	class VkSwapChain
	{
		VkSwapchainKHR mHandle = VK_NULL_HANDLE;

		ImageExtent mExtent;
		VkFormat mImageFormat;

		uint32 mFrameIndex = 0u;
		uint32 mImageIndex = 0u;

		std::vector<VkImage> mImages;
		std::vector<VkImageView> mImageViews;

		std::vector<VkFramebuffer> mFrameBuffers;
		std::vector<VkCommandBuffer> mGraphicsCommandBuffers;

		std::vector<VkSemaphore> mAcquireSemaphores;
		std::vector<VkSemaphore> mPresentSemaphores;

		std::vector<VkFence> mMainFences;

		VkDescriptorPool mDescriptorPool = VK_NULL_HANDLE;
		VkDescriptorSetLayout mDescriptorSetLayout = VK_NULL_HANDLE;
		std::vector<VkDescriptorSet> mDescriptorSets;

		std::vector<VkBuffer> mUniformBuffers;

		struct SupportDetails
		{
			VkSurfaceCapabilitiesKHR capabilities;
			std::vector<VkSurfaceFormatKHR> formats;
			std::vector<VkPresentModeKHR> presentModes;
		};

		static SupportDetails QuerySupportDetails(VkPhysicalDevice _device, const VkRenderSurface& _surface);
		VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
		ImageExtent ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

	public:
		uint32 GetImageNum() const noexcept;
		VkFormat GetImageFormat() const noexcept;

		const ImageExtent& GetImageExtent() const noexcept;

		VkDescriptorSetLayout GetDescriptorSetLayout() const noexcept;

		VkRenderFrame GetRenderFrame() const noexcept;

		void Create(const VkDevice& _device, const VkRenderSurface& _surface, const VkQueueFamilyIndices& _queueFamilyIndices);
		void Destroy(const VkDevice& _device);

		// Optimized re-creation.
		void ReCreate(const VkDevice& _device, const VkRenderSurface& _surface, const VkQueueFamilyIndices& _queueFamilyIndices);

		void CreateFrameBuffers(const VkDevice& _device, const VkRenderPass& _renderPass);
		void DestroyFrameBuffers(const VkDevice& _device);

		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API VkRenderFrame Update(const VkDevice& _device);

		static bool CheckSupport(VkPhysicalDevice _device, const VkRenderSurface& _surface);
	};
}

#endif

#endif // GUARD
