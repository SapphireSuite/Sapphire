// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Vulkan/System/Surface/VkSwapChain.hpp>

#include <Core/Algorithms/SizeOf.hpp>

#include <Rendering/Vulkan/System/VkMacro.hpp>
#include <Rendering/Vulkan/System/Device/VkDevice.hpp>
#include <Rendering/Vulkan/System/Surface/VkRenderSurface.hpp>

namespace Sa::Vk
{
	RenderFormat SwapChain::GetRenderFormat() const noexcept
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

		mFormat = API_FromRenderFormat(surfaceFormat.format);

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

	void SwapChain::DestroySurfaceRenderPass(const Device& _device)
	{
		for (auto it = mSurfaceRenderPassInfos.begin(); it != mSurfaceRenderPassInfos.end(); ++it)
		{
			for (auto fbIt = it->frameBuffers.begin(); fbIt != it->frameBuffers.end(); ++fbIt)
				fbIt->Destroy(_device);

			it->frameBuffers.clear();
		}

		mSurfaceRenderPassInfos.clear();
	}

	void SwapChain::Create(const Device& _device, const RenderSurface& _surface)
	{
		CreateSwapChainKHR(_device, _surface);
		CreateSynchronisation(_device);
	}

	void SwapChain::Destroy(const Device& _device)
	{
		DestroySurfaceRenderPass(_device);
		DestroySynchronisation(_device);
		DestroySwapChainKHR(_device);
	}

	void SwapChain::Begin(const Device& _device)
	{
		// Wait current Fence.
		vkWaitForFences(_device, 1, &mFramesSynch[mFrameIndex].fence, true, UINT64_MAX);

		// Reset current Fence.
		vkResetFences(_device, 1, &mFramesSynch[mFrameIndex].fence);

		SA_VK_ASSERT(vkAcquireNextImageKHR(_device, mHandle, UINT64_MAX, mFramesSynch[mFrameIndex].acquireSemaphore, VK_NULL_HANDLE, &mImageIndex),
			LibCommandFailed, Rendering, L"Failed to aquire next image!");
	}
	
	void SwapChain::End(const Device& _device)
	{

		// Query all command buffers.
		std::vector<VkCommandBuffer> commands;
		commands.reserve(mSurfaceRenderPassInfos.size());

		for (auto it = mSurfaceRenderPassInfos.begin(); it != mSurfaceRenderPassInfos.end(); ++it)
		{
			// Get current commandBuffer of renderpass infos.
			CommandBuffer& commandBuffer = it->frameBuffers[mFrameIndex].commandBuffer;
			commands.push_back(commandBuffer);
		}


		// Submit graphics.
		const VkPipelineStageFlags waitStages = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.pNext = nullptr;
		submitInfo.waitSemaphoreCount = 1u;
		submitInfo.pWaitSemaphores = &mFramesSynch[mFrameIndex].acquireSemaphore;
		submitInfo.pWaitDstStageMask = &waitStages;
		submitInfo.commandBufferCount = SizeOf(commands);
		submitInfo.pCommandBuffers = commands.data();
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

	FrameBuffer& SwapChain::GetFrameBuffer(uint32 _renderPassID)
	{
		for (auto it = mSurfaceRenderPassInfos.begin(); it != mSurfaceRenderPassInfos.end(); ++it)
		{
			if (it->ID == _renderPassID)
			{
				SA_ASSERT(mFrameIndex < SizeOf(it->frameBuffers), InvalidParam, Rendering, L"Invalid frame index!");
				return it->frameBuffers[mFrameIndex];
			}
		}

		SA_ASSERT(false, InvalidParam, Rendering, L"Invalid RenderPass ID");
		return mSurfaceRenderPassInfos[0].frameBuffers[0];
	}

	uint32 SwapChain::AddRenderPass(Device& _device, const RenderPass& _renderPass, const RenderPassDescriptor& _rpDesc)
	{
		static uint32 currID = 0u;

		std::vector<VkImage> swapChainImages(mImageNum);
		vkGetSwapchainImagesKHR(_device, mHandle, &mImageNum, swapChainImages.data());

		SurfaceRenderPassInfos& surfaceRenderPassInfos = mSurfaceRenderPassInfos.emplace_back();
		surfaceRenderPassInfos.frameBuffers.reserve(mImageNum);

		for (uint32 i = 0u; i < mImageNum; ++i)
		{
			FrameBuffer& frameBuffer = surfaceRenderPassInfos.frameBuffers.emplace_back();
			frameBuffer.Create(_device, _renderPass, _rpDesc, mExtent, i, swapChainImages[i]);
		}

		surfaceRenderPassInfos.ID = ++currID;
		return currID;
	}
	
	void SwapChain::RemoveRenderPass(Device& _device, uint32 _renderPassID)
	{
		for (auto it = mSurfaceRenderPassInfos.begin(); it != mSurfaceRenderPassInfos.end(); ++it)
		{
			// ID found.
			if (it->ID == _renderPassID)
			{
				// Destroy associated framebuffers.
				for (auto fbIt = it->frameBuffers.begin(); fbIt != it->frameBuffers.end(); ++fbIt)
					fbIt->Destroy(_device);

				// Erase render pass infos.
				mSurfaceRenderPassInfos.erase(it);
				return;
			}
		}

		SA_ASSERT(false, InvalidParam, Rendering, L"RenderPass ID not registered!");
	}
}
