// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Vulkan/System/Surface/VkSwapChain.hpp>

#include <Core/Algorithms/SizeOf.hpp>

#include <Rendering/Vulkan/System/VkMacro.hpp>
#include <Rendering/Vulkan/System/Device/VkDevice.hpp>
#include <Rendering/Vulkan/System/Surface/VkRenderSurface.hpp>

namespace Sa::Vk
{
	Format SwapChain::GetFormat() const noexcept
	{
		return mFormat;
	}

	void SwapChain::CreateSwapChainKHR(const Device& _device, const RenderSurface& _surface)
	{
		// Query infos.
		RenderSurface::SupportDetails details = _surface.QuerySupportDetails(_device);
		VkSurfaceFormatKHR surfaceFormat = RenderSurface::ChooseSwapSurfaceFormat(details);
		VkPresentModeKHR presentMode = RenderSurface::ChooseSwapPresentMode(details);
		mExtent = RenderSurface::ChooseSwapExtent(details);

		mFormat = API_FromFormat(surfaceFormat.format);

		// Min image count to avoid driver blocking.
		mImageNum = details.capabilities.minImageCount + 1;

		if (details.capabilities.maxImageCount > 0 && mImageNum > details.capabilities.maxImageCount)
			mImageNum = details.capabilities.maxImageCount;


		// Create Swapchain.
		VkSwapchainCreateInfoKHR swapChainCreateInfo{};
		swapChainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapChainCreateInfo.pNext = nullptr;
		swapChainCreateInfo.flags = 0u;
		swapChainCreateInfo.surface = _surface;
		swapChainCreateInfo.minImageCount = mImageNum;
		swapChainCreateInfo.imageFormat = surfaceFormat.format;
		swapChainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;
		swapChainCreateInfo.imageExtent = VkExtent2D{ mExtent.x, mExtent.y };
		swapChainCreateInfo.imageArrayLayers = 1u;
		swapChainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		swapChainCreateInfo.queueFamilyIndexCount = 0u;
		swapChainCreateInfo.pQueueFamilyIndices = nullptr;
		swapChainCreateInfo.preTransform = details.capabilities.currentTransform;
		swapChainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		swapChainCreateInfo.presentMode = presentMode;
		swapChainCreateInfo.clipped = VK_TRUE;
		swapChainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

		// Queue family setup.
		uint32 familyIndices[]
		{
			_device.queueMgr.graphics.GetFamilyIndex(),
			_device.queueMgr.present.GetFamilyIndex(),
		};

		if (_device.queueMgr.present.IsValid() && familyIndices[0] != familyIndices[1]) // Graphic and present familiy are different.
		{
			swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			swapChainCreateInfo.queueFamilyIndexCount = SizeOf(familyIndices);
			swapChainCreateInfo.pQueueFamilyIndices = familyIndices;
		}

		SA_VK_ASSERT(vkCreateSwapchainKHR(_device, &swapChainCreateInfo, nullptr, &mHandle),
			CreationFailed, Rendering, L"Failed to create swap chain!");
	}

	void SwapChain::DestroySwapChainKHR(const Device& _device)
	{
		SA_ASSERT(mHandle != VK_NULL_HANDLE, InvalidParam, Rendering, L"Destroy a null swapchain!");

		vkDestroySwapchainKHR(_device, mHandle, nullptr);
		mHandle = VK_NULL_HANDLE;
	}

	void SwapChain::CreateSynchronisation(const Device& _device)
	{
		VkSemaphoreCreateInfo semaphoreCreateInfo{};
		semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		semaphoreCreateInfo.pNext = nullptr;
		semaphoreCreateInfo.flags = 0u;

		VkFenceCreateInfo fenceCreateInfo{};
		fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceCreateInfo.pNext = nullptr;
		fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		mFramesSynch.resize(mImageNum);

		for (auto it = mFramesSynch.begin(); it != mFramesSynch.end(); ++it)
		{
			SA_VK_ASSERT(vkCreateSemaphore(_device, &semaphoreCreateInfo, nullptr, &it->acquireSemaphore),
				CreationFailed, Rendering, L"Failed to create semaphore!");
			SA_VK_ASSERT(vkCreateSemaphore(_device, &semaphoreCreateInfo, nullptr, &it->presentSemaphore),
				CreationFailed, Rendering, L"Failed to create semaphore!");

			SA_VK_ASSERT(vkCreateFence(_device, &fenceCreateInfo, nullptr, &it->fence),
				CreationFailed, Rendering, L"Failed to create fence!");
		}
	}
	
	void SwapChain::DestroySynchronisation(const Device& _device)
	{
		for (auto it = mFramesSynch.begin(); it != mFramesSynch.end(); ++it)
		{
			vkDestroySemaphore(_device, it->acquireSemaphore, nullptr);
			vkDestroySemaphore(_device, it->presentSemaphore, nullptr);
			vkDestroyFence(_device, it->fence, nullptr);
		}

		mFramesSynch.clear();
	}

	void SwapChain::Create(const Device& _device, const RenderSurface& _surface)
	{
		CreateSwapChainKHR(_device, _surface);
		CreateSynchronisation(_device);
	}

	void SwapChain::Destroy(const Device& _device)
	{
		DestroyFrameBuffers(_device);

		DestroySynchronisation(_device);
		DestroySwapChainKHR(_device);
	}

	RenderFrame SwapChain::Begin(const Device& _device)
	{
		// Wait current Fence.
		vkWaitForFences(_device, 1, &mFramesSynch[mFrameIndex].fence, true, UINT64_MAX);

		// Reset current Fence.
		vkResetFences(_device, 1, &mFramesSynch[mFrameIndex].fence);

		SA_VK_ASSERT(vkAcquireNextImageKHR(_device, mHandle, UINT64_MAX, mFramesSynch[mFrameIndex].acquireSemaphore, VK_NULL_HANDLE, &mImageIndex),
			LibCommandFailed, Rendering, L"Failed to aquire next image!");


		mFrameBuffers[mFrameIndex]->Begin();

		return RenderFrame{ mFrameIndex, *mFrameBuffers[mFrameIndex] };
	}
	
	void SwapChain::End(const Device& _device)
	{
		mFrameBuffers[mFrameIndex]->End();


		// Submit graphics.
		const VkPipelineStageFlags waitStages = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.pNext = nullptr;
		submitInfo.waitSemaphoreCount = 1u;
		submitInfo.pWaitSemaphores = &mFramesSynch[mFrameIndex].acquireSemaphore;
		submitInfo.pWaitDstStageMask = &waitStages;
		submitInfo.commandBufferCount = 1u;
		submitInfo.pCommandBuffers = &mFrameBuffers[mFrameIndex]->commandBuffer.Get();
		submitInfo.signalSemaphoreCount = 1u;
		submitInfo.pSignalSemaphores = &mFramesSynch[mFrameIndex].presentSemaphore;

		SA_VK_ASSERT(vkQueueSubmit(_device.queueMgr.graphics.GetHandle(mFrameIndex), 1, &submitInfo, mFramesSynch[mFrameIndex].fence),
			LibCommandFailed, Rendering, L"Failed to submit graphics queue!");


		// Submit present.
		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.pNext = nullptr;
		presentInfo.waitSemaphoreCount = 1u;
		presentInfo.pWaitSemaphores = &mFramesSynch[mFrameIndex].presentSemaphore;
		presentInfo.swapchainCount = 1u;
		presentInfo.pSwapchains = &mHandle;
		presentInfo.pImageIndices = &mImageIndex;
		presentInfo.pResults = nullptr;

		SA_VK_ASSERT(vkQueuePresentKHR(_device.queueMgr.present.GetHandle(mFrameIndex), &presentInfo),
			LibCommandFailed, Rendering, L"Failed to submit present queue!");


		// Increment next frame.
		mFrameIndex = (mFrameIndex + 1) % mImageNum;
	}

	const std::vector<FrameBuffer*>& SwapChain::CreateFrameBuffers(const Device& _device, const RenderPass& _renderPass,
		const RenderPassDescriptor& _renderPassDesc, uint32* _size)
	{
		std::vector<VkImage> swapChainImages(mImageNum);
		vkGetSwapchainImagesKHR(_device, mHandle, &mImageNum, swapChainImages.data());

		mFrameBuffers.reserve(mImageNum);

		for (uint32 i = 0u; i < mImageNum; ++i)
		{
			FrameBuffer* frameBuffer = mFrameBuffers.emplace_back(new FrameBuffer());
			frameBuffer->Create(_device, _renderPass, _renderPassDesc, mExtent, i, swapChainImages[i]);
		}

		if (_size)
			*_size = mImageNum;

		return mFrameBuffers;
	}

	void SwapChain::DestroyFrameBuffers(const Device& _device)
	{
		for (auto it = mFrameBuffers.begin(); it != mFrameBuffers.end(); ++it)
		{
			(*it)->Destroy(_device);
			delete* it;
		}

		mFrameBuffers.clear();
	}
}
