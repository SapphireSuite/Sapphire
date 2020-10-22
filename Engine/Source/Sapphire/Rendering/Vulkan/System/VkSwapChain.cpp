// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Vulkan/System/VkSwapChain.hpp>

#include <Core/Algorithms/SizeOf.hpp>

#include <Maths/Misc/Maths.hpp>

#include <Rendering/Vulkan/System/VkMacro.hpp>
#include <Rendering/Vulkan/System/VkDevice.hpp>
#include <Rendering/Vulkan/System/VkRenderSurface.hpp>

#include <Rendering/Framework/Primitives/Material/UniformBuffers.hpp>

#include <Rendering/Vulkan/System/Framebuffer.hpp>

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

	const ImageExtent& VkSwapChain::GetImageExtent() const noexcept
	{
		return mExtent;
	}

	const VkRenderFrame VkSwapChain::GetRenderFrame() const noexcept
	{
		return VkRenderFrame{ mImageIndex, &mFrames[mImageIndex] };
	}

	void VkSwapChain::Create(const VkDevice& _device, const VkRenderSurface& _surface,
								const VkQueueFamilyIndices& _queueFamilyIndices, const RenderPass& _renderPass)
	{
		CreateSwapChainKHR(_device, _surface, _queueFamilyIndices);
		CreateFrames(_device, _renderPass);
		CreateSynchronisation(_device);
	}
	void VkSwapChain::Destroy(const VkDevice& _device)
	{
		DestroySynchronisation(_device);
		DestroyFrames(_device);
		DestroySwapChainKHR(_device);
	}

	void VkSwapChain::ReCreate(const VkDevice& _device, const VkRenderSurface& _surface, const VkQueueFamilyIndices& _queueFamilyIndices)
	{
		Destroy(_device);

		//Create(_device, _surface, _queueFamilyIndices);
	}

	VkRenderFrame VkSwapChain::Begin(const VkDevice& _device)
	{
		// Wait current Fence.
		vkWaitForFences(_device, 1, &mFramesSynchronisation[mFrameIndex].fence, true, UINT64_MAX);

		// Reset current Fence.
		vkResetFences(_device, 1, &mFramesSynchronisation[mFrameIndex].fence);

		SA_VK_ASSERT(vkAcquireNextImageKHR(_device, mHandle, UINT64_MAX, mFramesSynchronisation[mFrameIndex].acquireSemaphore, VK_NULL_HANDLE, &mImageIndex),
			LibCommandFailed, Rendering, L"Failed to aquire next image!");

		VkRenderFrame frame = GetRenderFrame();
		frame.framebuffer->Begin();
		return frame;
	}
	
	void VkSwapChain::End(const VkDevice& _device)
	{
		// Get current frame components.
		VkRenderFrame frame = GetRenderFrame();

		frame.framebuffer->End();

		const VkPipelineStageFlags waitStages = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

		// Submit previous graphic.
		const VkSubmitInfo submitInfo
		{
			VK_STRUCTURE_TYPE_SUBMIT_INFO,						// sType.
			nullptr,											// pNext.
			1,													// waitSemaphoreCount.
			&mFramesSynchronisation[mFrameIndex].acquireSemaphore,					// pWaitSemaphores.
			&waitStages,										// pWaitDstStageMask.
			1,													// commandBufferCount.
			&frame.framebuffer->GetCommandBuffer().Get(),					// pCommandBuffers.
			1,													// signalSemaphoreCount.
			&mFramesSynchronisation[mFrameIndex].presentSemaphore,					// pSignalSemaphores.
		};

		SA_VK_ASSERT(vkQueueSubmit(_device.GetGraphicsQueue(), 1, &submitInfo, mFramesSynchronisation[mFrameIndex].fence),
			LibCommandFailed, Rendering, L"Failed to submit graphics queue!");

		// Submit previous present.
		const VkPresentInfoKHR presentInfo
		{
			VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,					// sType.
			nullptr,											// pNext.
			1,													// waitSemaphoreCount.
			&mFramesSynchronisation[mFrameIndex].presentSemaphore,					// pWaitSemaphores.
			1,													// swapchainCount.
			&mHandle,											// pSwapchains
			&mImageIndex,										// pImageIndices.
			nullptr												// pResults.
		};

		SA_VK_ASSERT(vkQueuePresentKHR(_device.GetPresentQueue(), &presentInfo),
			LibCommandFailed, Rendering, L"Failed to submit present queue!");

		// Increment next frame.
		mFrameIndex = (mFrameIndex + 1) % mFramesSynchronisation.size();
	}

	void VkSwapChain::CreateSwapChainKHR(const VkDevice& _device, const VkRenderSurface& _surface, const VkQueueFamilyIndices& _queueFamilyIndices)
	{
		// Query infos.
		VkSurfaceFormatKHR surfaceFormat = _surface.ChooseSwapSurfaceFormat();
		VkPresentModeKHR presentMode = _surface.ChooseSwapPresentMode();

		mImageFormat = surfaceFormat.format;
		mExtent = _surface.ChooseSwapExtent();


		// Min image count to avoid driver blocking.
		uint32 imageNum = _surface.GetSupportDetails().capabilities.minImageCount + 1;

		if (_surface.GetSupportDetails().capabilities.maxImageCount > 0 
				&& imageNum > _surface.GetSupportDetails().capabilities.maxImageCount)
			imageNum = _surface.GetSupportDetails().capabilities.maxImageCount;

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
			_surface.GetSupportDetails().capabilities.currentTransform,		// preTransform.
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
	}

	void VkSwapChain::DestroySwapChainKHR(const VkDevice& _device)
	{
		vkDestroySwapchainKHR(_device, mHandle, nullptr);
		mHandle = VK_NULL_HANDLE;
	}

	void VkSwapChain::CreateFrames(const VkDevice& _device, const RenderPass& _renderPass)
	{
		// Create images.
		uint32_t imageNum;
		vkGetSwapchainImagesKHR(_device, mHandle, &imageNum, nullptr);
		std::vector<VkImage> imgs(imageNum);
		vkGetSwapchainImagesKHR(_device, mHandle, &imageNum, imgs.data());

		mFrames.reserve(imageNum);
		for (size_t i = 0; i < imgs.size(); i++)
		{
			VkImageBufferCreateInfos colorBufferCreateInfos{};
			colorBufferCreateInfos.format = _renderPass.GetColorFormat();
			colorBufferCreateInfos.extent = mExtent;
			colorBufferCreateInfos.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT;
			colorBufferCreateInfos.mipMapLevels = 1;
			colorBufferCreateInfos.sampleCount = static_cast<VkSampleCountFlagBits>(SampleBits::Sample1Bit);
			colorBufferCreateInfos.aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;

			VkImageBuffer imageBuffer{};
			imageBuffer.CreateFromImage(_device, colorBufferCreateInfos, imgs[i]);
			mFrames.emplace_back(&_renderPass, mExtent, imageBuffer);
		}
	}

	void VkSwapChain::DestroyFrames(const VkDevice& _device)
	{

	}

	void VkSwapChain::CreateSynchronisation(const VkDevice& _device)
	{
		const VkSemaphoreCreateInfo semaphoreCreateInfo
		{
			VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,					// sType.
			nullptr,													// pNext.
			0,															// flags.
		};

		const VkFenceCreateInfo fenceCreateInfo
		{
			VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,					// sType.
			nullptr,												// pNext.
			VK_FENCE_CREATE_SIGNALED_BIT							// flags.
		};

		const int framesInFlight = 2;

		mFramesSynchronisation.resize(framesInFlight);
		for (size_t i = 0; i < framesInFlight; i++)
		{
			SA_VK_ASSERT(vkCreateSemaphore(_device, &semaphoreCreateInfo, nullptr, &mFramesSynchronisation[i].acquireSemaphore),
				CreationFailed, Rendering, L"Failed to create semaphore!");
			SA_VK_ASSERT(vkCreateSemaphore(_device, &semaphoreCreateInfo, nullptr, &mFramesSynchronisation[i].presentSemaphore),
				CreationFailed, Rendering, L"Failed to create semaphore!");

			SA_VK_ASSERT(vkCreateFence(_device, &fenceCreateInfo, nullptr, &mFramesSynchronisation[i].fence),
				CreationFailed, Rendering, L"Failed to create fence!");
		}
	}

	void VkSwapChain::DestroySynchronisation(const VkDevice& _device)
	{
		for (uint32 i = 0; i < mFramesSynchronisation.size(); i++)
		{
			vkDestroySemaphore(_device, mFramesSynchronisation[i].acquireSemaphore, nullptr);
			vkDestroySemaphore(_device, mFramesSynchronisation[i].presentSemaphore, nullptr);
			vkDestroyFence(_device, mFramesSynchronisation[i].fence, nullptr);
		}
	}
}

#endif
