// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Vulkan/System/VkSwapChain.hpp>

#include <Core/Algorithms/SizeOf.hpp>

#include <Maths/Misc/Maths.hpp>

#include <Rendering/Vulkan/System/VkMacro.hpp>
#include <Rendering/Vulkan/System/VkDevice.hpp>
#include <Rendering/Vulkan/System/VkRenderPass.hpp>
#include <Rendering/Vulkan/System/VkRenderSurface.hpp>

#include <Rendering/Framework/Primitives/Material/UniformBuffers.hpp>

#include <Rendering/Vulkan/Buffer/VkCommandBuffer.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa
{
	uint32 VkSwapChain::GetImageNum() const noexcept
	{
		return SizeOf(mFrames);
	}

	VkFormat VkSwapChain::GetImageFormat() const noexcept
	{
		return mImageFormat;
	}

	VkImageView VkSwapChain::GetImageView(uint32 _index) const noexcept
	{
		SA_ASSERT(_index < SizeOf(mFrames), OutOfRange, Rendering, _index, 0, SizeOf(mFrames));

		return mFrames[_index].imageView;
	}

	const ImageExtent& VkSwapChain::GetImageExtent() const noexcept
	{
		return mExtent;
	}

	VkRenderFrame VkSwapChain::GetRenderFrame() const noexcept
	{
		return mFrames[mImageIndex];
	}

	void VkSwapChain::Create(const VkDevice& _device, const VkRenderSurface& _surface, const VkQueueFamilyIndices& _queueFamilyIndices)
	{
		CreateSwapChainKHR(_device, _surface, _queueFamilyIndices);
		CreateImageView(_device);
		
		CreateCommandBuffers(_device);

		CreateSemaphores(_device);
		CreateFences(_device);
	}
	void VkSwapChain::Destroy(const VkDevice& _device)
	{
		DestroyFences(_device);
		DestroySemaphores(_device);

		DestroyCommandBuffers(_device);

		DestroyImageView(_device);
		DestroySwapChainKHR(_device);
	}

	void VkSwapChain::ReCreate(const VkDevice& _device, const VkRenderSurface& _surface, const VkQueueFamilyIndices& _queueFamilyIndices)
	{
		Destroy(_device);

		Create(_device, _surface, _queueFamilyIndices);
	}

	VkRenderFrame VkSwapChain::Begin(const VkDevice& _device)
	{
		// Get new current frame components.
		VkRenderFrame frame = GetRenderFrame();


		// Wait current Fence.
		vkWaitForFences(_device, 1, &frame.fence, true, UINT64_MAX);

		// Reset current Fence.
		vkResetFences(_device, 1, &frame.fence);

		// Reset Command Buffer.
		vkResetCommandBuffer(frame.graphicsCommandBuffer, VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);

		SA_VK_ASSERT(vkAcquireNextImageKHR(_device, mHandle, UINT64_MAX, frame.acquireSemaphore, VK_NULL_HANDLE, &mImageIndex),
			LibCommandFailed, Rendering, L"Failed to aquire next image!");

		const VkCommandBufferBeginInfo commandBufferBeginInfo
		{
			VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,		// sType.
			nullptr,											// pNext.
			VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,		// flags.
			nullptr,											// pInheritanceInfo.
		};

		SA_VK_ASSERT(vkBeginCommandBuffer(frame.graphicsCommandBuffer, &commandBufferBeginInfo),
			LibCommandFailed, Rendering, L"Failed to begin command buffer!");


		return frame;
	}
	
	void VkSwapChain::End(const VkDevice& _device)
	{
		// Get current frame components.
		VkRenderFrame frame = GetRenderFrame();

		SA_VK_ASSERT(vkEndCommandBuffer(frame.graphicsCommandBuffer),
			LibCommandFailed, Rendering, L"Failed to end command buffer!");


		const VkPipelineStageFlags waitStages = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

		// Submit previous graphic.
		const VkSubmitInfo submitInfo
		{
			VK_STRUCTURE_TYPE_SUBMIT_INFO,						// sType.
			nullptr,											// pNext.
			1,													// waitSemaphoreCount.
			&frame.acquireSemaphore,					// pWaitSemaphores.
			&waitStages,										// pWaitDstStageMask.
			1,													// commandBufferCount.
			&frame.graphicsCommandBuffer.Get(),					// pCommandBuffers.
			1,													// signalSemaphoreCount.
			&frame.presentSemaphore,					// pSignalSemaphores.
		};

		SA_VK_ASSERT(vkQueueSubmit(_device.GetGraphicsQueue(), 1, &submitInfo, frame.fence),
			LibCommandFailed, Rendering, L"Failed to submit graphics queue!");

		// Submit previous present.
		const VkPresentInfoKHR presentInfo
		{
			VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,					// sType.
			nullptr,											// pNext.
			1,													// waitSemaphoreCount.
			&frame.presentSemaphore,					// pWaitSemaphores.
			1,													// swapchainCount.
			&mHandle,											// pSwapchains
			&mImageIndex,										// pImageIndices.
			nullptr												// pResults.
		};

		SA_VK_ASSERT(vkQueuePresentKHR(_device.GetPresentQueue(), &presentInfo),
			LibCommandFailed, Rendering, L"Failed to submit present queue!");

		// Increment next frame.
		mFrameIndex = (mFrameIndex + 1) % GetImageNum();
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

	void VkSwapChain::CreateSwapChainKHR(const VkDevice& _device, const VkRenderSurface& _surface, const VkQueueFamilyIndices& _queueFamilyIndices)
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
		mFrames.resize(imageNum);
		std::vector<VkImage> imgs(imageNum);
		vkGetSwapchainImagesKHR(_device, mHandle, &imageNum, imgs.data());

		for (size_t i = 0; i < imgs.size(); i++)
		{
			mFrames[i].index = i;
			mFrames[i].image = imgs[i];
		}
	}
	void VkSwapChain::DestroySwapChainKHR(const VkDevice& _device)
	{
		vkDestroySwapchainKHR(_device, mHandle, nullptr);
		mHandle = VK_NULL_HANDLE;
	}

	void VkSwapChain::CreateImageView(const VkDevice& _device)
	{
		for (uint32 i = 0; i < mFrames.size(); i++)
		{
			const VkImageViewCreateInfo imageViewCreateInfo
			{
				VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,									// sType.
				nullptr,																	// pNext.
				0,																			// flags.
				mFrames[i].image,															// image.
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

			SA_VK_ASSERT(vkCreateImageView(_device, &imageViewCreateInfo, nullptr, &mFrames[i].imageView),
				CreationFailed, Rendering, L"Failed to create image views!");
		}
	}
	void VkSwapChain::DestroyImageView(const VkDevice& _device)
	{
		for (uint32 i = 0; i < mFrames.size(); i++)
			vkDestroyImageView(_device, mFrames[i].imageView, nullptr);
	}

	void VkSwapChain::CreateCommandBuffers(const VkDevice& _device)
	{
		const VkCommandBufferAllocateInfo commandBufferAllocInfo
		{
			VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,							// sType.
			nullptr,																// nullptr.
			_device.GetGraphicsQueue().GetCommandPool(),							// commandPool.
			VK_COMMAND_BUFFER_LEVEL_PRIMARY,										// level.
			1																// commandBufferCount.
		};

		for (size_t i = 0; i < mFrames.size(); ++i)
		{
			SA_VK_ASSERT(vkAllocateCommandBuffers(_device, &commandBufferAllocInfo, &mFrames[i].graphicsCommandBuffer.Get()),
				CreationFailed, Rendering, L"Failed to allocate command buffers!");
		}
	}
	void VkSwapChain::DestroyCommandBuffers(const VkDevice& _device) 
	{
		// Manually free command buffers (useful for resize).
		for (size_t i = 0; i < mFrames.size(); ++i)
			vkFreeCommandBuffers(_device, _device.GetGraphicsQueue().GetCommandPool(),
								1, &mFrames[i].graphicsCommandBuffer.Get());
	}

	void VkSwapChain::CreateSemaphores(const VkDevice& _device)
	{
		const VkSemaphoreCreateInfo semaphoreCreateInfo
		{
			VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,					// sType.
			nullptr,													// pNext.
			0,															// flags.
		};

		for (uint32 i = 0u; i < mFrames.size(); ++i)
		{
			SA_VK_ASSERT(vkCreateSemaphore(_device, &semaphoreCreateInfo, nullptr, &mFrames[i].acquireSemaphore),
				CreationFailed, Rendering, L"Failed to create semaphore!");
			SA_VK_ASSERT(vkCreateSemaphore(_device, &semaphoreCreateInfo, nullptr, &mFrames[i].presentSemaphore),
				CreationFailed, Rendering, L"Failed to create semaphore!");
		}
	}
	void VkSwapChain::DestroySemaphores(const VkDevice& _device)
	{
		for (uint32 i = 0; i < mFrames.size(); i++)
		{
			vkDestroySemaphore(_device, mFrames[i].acquireSemaphore, nullptr);
			vkDestroySemaphore(_device, mFrames[i].presentSemaphore, nullptr);
		}
	}

	void VkSwapChain::CreateFences(const VkDevice& _device)
	{
		const VkFenceCreateInfo fenceCreateInfo
		{
			VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,					// sType.
			nullptr,												// pNext.
			VK_FENCE_CREATE_SIGNALED_BIT							// flags.
		};

		for (uint32 i = 0u; i < mFrames.size(); ++i)
		{
			SA_VK_ASSERT(vkCreateFence(_device, &fenceCreateInfo, nullptr, &mFrames[i].fence),
				CreationFailed, Rendering, L"Failed to create fence!");
		}
	}
	void VkSwapChain::DestroyFences(const VkDevice& _device)
	{
		for (uint32 i = 0; i < mFrames.size(); i++)
			vkDestroyFence(_device, mFrames[i].fence, nullptr);

	}
}

#endif
