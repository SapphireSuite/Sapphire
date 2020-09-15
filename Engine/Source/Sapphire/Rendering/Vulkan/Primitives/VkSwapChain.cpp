// Copyright 2020 Sapphire development team. All Rights Reserved.


#include <Rendering/Vulkan/Primitives/VkSwapChain.hpp>

#include <Maths/Misc/Maths.hpp>

#include <Rendering/Vulkan/VkMacro.hpp>
#include <Rendering/Vulkan/Primitives/VkDevice.hpp>
#include <Rendering/Vulkan/Primitives/VkRenderPass.hpp>
#include <Rendering/Vulkan/Primitives/VkRenderSurface.hpp>

#include <Rendering/Vulkan/Queue/VkQueueFamilyIndices.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa
{
	uint32 VkSwapChain::GetImageNum() const noexcept
	{
		return static_cast<uint32>(mImages.size());
	}

	VkFormat VkSwapChain::GetImageFormat() const noexcept
	{
		return mImageFormat;
	}

	const ImageExtent& VkSwapChain::GetImageExtent() const noexcept
	{
		return mExtent;
	}

	VkRenderFrame VkSwapChain::GetRenderFrame() const noexcept
	{
		return VkRenderFrame
		{
			mImages[mImageIndex],
			mImageViews[mImageIndex],
			mFrameBuffers[mFrameIndex],
			mGraphicsCommandBuffers[mFrameIndex],
			mAcquireSemaphores[mFrameIndex],
			mPresentSemaphores[mFrameIndex],
			mMainFences[mFrameIndex],
		};
	}

	void VkSwapChain::Create(const VkDevice& _device, const VkRenderSurface& _surface, const VkQueueFamilyIndices& _queueFamilyIndices)
	{
		// Query infos.
		SupportDetails swapChainSupport = QuerySupportDetails(_device, _surface);

		VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.formats);
		VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.presentModes);

		mImageFormat = surfaceFormat.format;
		mExtent = ChooseSwapExtent(swapChainSupport.capabilities);


		// Min image count to avoid driver blocking.
		uint32 imageNum = swapChainSupport.capabilities.minImageCount + 1;

		if (swapChainSupport.capabilities.maxImageCount > 0 && imageNum > swapChainSupport.capabilities.maxImageCount)
			imageNum = swapChainSupport.capabilities.maxImageCount;


		VkSwapchainCreateInfoKHR swapChainCreateInfo
		{
			VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,					// sType.
			nullptr,														// pNext.
			0,																// flags.
			_surface,														// surface.
			imageNum,														// minImageCount.
			surfaceFormat.format,											// imageFormat.
			surfaceFormat.colorSpace,										// imageColorSpace.
			mExtent,														// imageExtent.
			1,																// imageArrayLayers.
			VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,							// imageUsage.
			VK_SHARING_MODE_EXCLUSIVE, 										// imageSharingMode.
			0,																// queueFamilyIndexCount.
			nullptr,														// pQueueFamilyIndices.
			swapChainSupport.capabilities.currentTransform,					// preTransform.
			VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,								// compositeAlpha.
			presentMode,													// presentMode.
			VK_TRUE,														// clipped.
			VK_NULL_HANDLE													// oldSwapchain.
		};

		if (_queueFamilyIndices.graphicsFamily != _queueFamilyIndices.presentFamily)
		{
			swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			swapChainCreateInfo.queueFamilyIndexCount = 2;
			swapChainCreateInfo.pQueueFamilyIndices = &_queueFamilyIndices.graphicsFamily;
		}

		SA_VK_ASSERT(vkCreateSwapchainKHR(_device, &swapChainCreateInfo, nullptr, &mHandle),
			CreationFailed, Rendering, L"Failed to create swap chain!");


		// Create images.
		vkGetSwapchainImagesKHR(_device, mHandle, &imageNum, nullptr);
		mImages.resize(imageNum);
		vkGetSwapchainImagesKHR(_device, mHandle, &imageNum, mImages.data());


		// Create Image views.
		mImageViews.resize(imageNum);

		for (uint32 i = 0; i < imageNum; i++)
		{
			const VkImageViewCreateInfo imageViewCreateInfo
			{
				VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,									// sType.
				nullptr,																	// pNext.
				VK_IMAGE_VIEW_CREATE_FRAGMENT_DENSITY_MAP_DYNAMIC_BIT_EXT,					// flags.
				mImages[i],																	// image.
				VK_IMAGE_VIEW_TYPE_2D,														// viewType.
				mImageFormat,																// format.

				VkComponentMapping															// components.
				{
					VK_COMPONENT_SWIZZLE_IDENTITY,										// r.
					VK_COMPONENT_SWIZZLE_IDENTITY,										// g.
					VK_COMPONENT_SWIZZLE_IDENTITY,										// b.
					VK_COMPONENT_SWIZZLE_IDENTITY										// a.
				},
				VkImageSubresourceRange														// subresourceRange.
				{
					VK_IMAGE_ASPECT_COLOR_BIT,											// aspectMask.
					0,																	// baseMipLevel.
					1,																	// levelCount.
					0,																	// baseArrayLayer.
					1																	// layerCount.
				}
			};

			SA_VK_ASSERT(vkCreateImageView(_device, &imageViewCreateInfo, nullptr, &mImageViews[i]),
				CreationFailed, Rendering, L"Failed to create image views!");
		}


		// Create Command Buffers.
		mGraphicsCommandBuffers.resize(imageNum);

		const VkCommandBufferAllocateInfo commandBufferAllocInfo
		{
			VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,							// sType.
			nullptr,																// nullptr.
			_device.GetGraphicsQueue().GetCommandPool(),							// commandPool.
			VK_COMMAND_BUFFER_LEVEL_PRIMARY,										// level.
			imageNum																// commandBufferCount.
		};

		SA_VK_ASSERT(vkAllocateCommandBuffers(_device, &commandBufferAllocInfo, mGraphicsCommandBuffers.data()),
			CreationFailed, Rendering, L"Failed to allocate command buffers!");


		// Semaphore Creation.
		mAcquireSemaphores.resize(imageNum);
		mPresentSemaphores.resize(imageNum);

		const VkSemaphoreCreateInfo semaphoreCreateInfo
		{
			VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,					// sType.
			nullptr,													// pNext.
			0,															// flags.
		};

		for (uint32 i = 0u; i < imageNum; ++i)
		{
			SA_VK_ASSERT(vkCreateSemaphore(_device, &semaphoreCreateInfo, nullptr, &mAcquireSemaphores[i]),
				CreationFailed, Rendering, L"Failed to create semaphore!");
			SA_VK_ASSERT(vkCreateSemaphore(_device, &semaphoreCreateInfo, nullptr, &mPresentSemaphores[i]),
				CreationFailed, Rendering, L"Failed to create semaphore!");
		}


		// Fence Creation.
		mMainFences.resize(imageNum);

		const VkFenceCreateInfo fenceCreateInfo
		{
			VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,					// sType.
			nullptr,												// pNext.
			VK_FENCE_CREATE_SIGNALED_BIT							// flags.
		};

		for (uint32 i = 0u; i < imageNum; ++i)
			SA_VK_ASSERT(vkCreateFence(_device, &fenceCreateInfo, nullptr, &mMainFences[i]),
				CreationFailed, Rendering, L"Failed to create fence!");
	}
	void VkSwapChain::Destroy(const VkDevice& _device)
	{
		DestroyFrameBuffers(_device);

		const uint32 imageNum = GetImageNum();

		for (uint32 i = 0; i < imageNum; i++)
		{
			vkDestroyFence(_device, mMainFences[i], nullptr);

			vkDestroySemaphore(_device, mAcquireSemaphores[i], nullptr);
			vkDestroySemaphore(_device, mPresentSemaphores[i], nullptr);

			vkDestroyImageView(_device, mImageViews[i], nullptr);
		}

		vkFreeCommandBuffers(_device, _device.GetGraphicsQueue().GetCommandPool(), imageNum, mGraphicsCommandBuffers.data());

		vkDestroySwapchainKHR(_device, mHandle, nullptr);
	}

	void VkSwapChain::CreateFrameBuffers(const VkDevice& _device, const VkRenderPass& _renderPass)
	{
		mFrameBuffers.resize(GetImageNum());

		for (uint32 i = 0u; i < mFrameBuffers.size(); ++i)
		{
			const VkFramebufferCreateInfo framebufferCreateInfo
			{
				VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,			// sType.
				nullptr,											// pNext.
				0,													// flags.
				_renderPass,							            // renderPass.
				1,									                // attachmentCount.
				&mImageViews[i],									// pAttachments.
				mExtent.width,										// width.
				mExtent.height,										// height.
				1													// layers.

			};

			SA_VK_ASSERT(vkCreateFramebuffer(_device, &framebufferCreateInfo, nullptr, &mFrameBuffers[i]),
				CreationFailed, Rendering, L"Failed to create framebuffer!");
		}
	}
	void VkSwapChain::DestroyFrameBuffers(const VkDevice& _device)
	{
		for (uint32 i = 0u; i < mFrameBuffers.size(); ++i)
			vkDestroyFramebuffer(_device, mFrameBuffers[i], nullptr);

		mFrameBuffers.clear();
	}

	VkSurfaceFormatKHR VkSwapChain::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
	{
		// Prefered
		for (uint32 i = 0; i < availableFormats.size(); ++i)
		{
			if (availableFormats[i].format == VK_FORMAT_B8G8R8A8_SRGB && availableFormats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
				return availableFormats[i];
		}

		// Default.
		return availableFormats[0];
	}

	VkPresentModeKHR VkSwapChain::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
	{
		// Prefered.
		for (uint32 i = 0; i < availablePresentModes.size(); ++i)
		{
			if (availablePresentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR)
				return availablePresentModes[i];
		}

		// Default FIFO always supported.
		return VK_PRESENT_MODE_FIFO_KHR;
	}

	ImageExtent VkSwapChain::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
	{
		if (capabilities.currentExtent.width != UINT32_MAX)
			return capabilities.currentExtent;
		else
		{
			// TODO: Clean.
			VkExtent2D actualExtent = { 1920, 1080 };

			actualExtent.width = Maths::Max(capabilities.minImageExtent.width, Maths::Min(capabilities.maxImageExtent.width, actualExtent.width));
			actualExtent.height = Maths::Max(capabilities.minImageExtent.height, Maths::Min(capabilities.maxImageExtent.height, actualExtent.height));

			return actualExtent;
		}
	}

	VkSwapChain::SupportDetails VkSwapChain::QuerySupportDetails(VkPhysicalDevice _device, const VkRenderSurface& _surface)
	{
		SupportDetails details;

		// KHR Capabilities.
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(_device, _surface, &details.capabilities);


		// KHR Formats.
		uint32 formatCount = 0u;
		vkGetPhysicalDeviceSurfaceFormatsKHR(_device, _surface, &formatCount, nullptr);

		if (formatCount != 0)
		{
			details.formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(_device, _surface, &formatCount, details.formats.data());
		}


		// KHR Present Modes.
		uint32 presentModeCount = 0u;
		vkGetPhysicalDeviceSurfacePresentModesKHR(_device, _surface, &presentModeCount, nullptr);

		if (presentModeCount != 0)
		{
			details.presentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(_device, _surface, &presentModeCount, details.presentModes.data());
		}

		return details;
	}

	bool VkSwapChain::CheckSupport(VkPhysicalDevice _device, const VkRenderSurface& _surface)
	{
		SupportDetails details = QuerySupportDetails(_device, _surface);

		return !details.formats.empty() && !details.presentModes.empty();
	}
}

#endif
