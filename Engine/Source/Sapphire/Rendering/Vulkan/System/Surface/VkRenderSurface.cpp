// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Vulkan/System/Surface/VkRenderSurface.hpp>

#include <Rendering/Vulkan/System/VkRenderInstance.hpp>
#include <Rendering/Vulkan/System/VkRenderPass.hpp>

namespace Sa::Vk
{
	RenderSurface::RenderSurface(VkSurfaceKHR _handle) noexcept : mHandle{ _handle }
	{
	}


	Format RenderSurface::GetFormat() const noexcept
	{
		return mSwapChain.GetFormat();
	}


	void RenderSurface::Create(const IRenderInstance& _instance)
	{
		SA_ASSERT(mHandle != VK_NULL_HANDLE, Nullptr, Rendering,
			L"Handle is nullptr. VkSurfaceKHR must be created first: use VkRenderInstance.CreateRenderSurface().");

		mSwapChain.Create(_instance, *this);
	}

	void RenderSurface::Destroy(const IRenderInstance& _instance)
	{
		SA_ASSERT(mHandle != VK_NULL_HANDLE, Nullptr, Rendering,
			L"Handle is nullptr. VkSurfaceKHR must be created first: use VkRenderInstance.CreateRenderSurface().");

		mSwapChain.Destroy(_instance);
	}

	RenderFrame RenderSurface::Begin(const IRenderInstance& _instance)
	{
		return mSwapChain.Begin(_instance.As<RenderInstance>().device);
	}
	
	void RenderSurface::End(const IRenderInstance& _instance)
	{
		mSwapChain.End(_instance.As<RenderInstance>().device);
	}

	const std::vector<IFrameBuffer*>& RenderSurface::CreateFrameBuffers(const IRenderInstance& _instance, const IRenderPass& _renderPass,
		const RenderPassDescriptor& _renderPassDesc, uint32* _size)
	{
		return reinterpret_cast<const std::vector<IFrameBuffer*>&>(mSwapChain.CreateFrameBuffers(
			_instance,
			_renderPass.As<RenderPass>(), _renderPassDesc,
			_size
		));
	}

	void RenderSurface::DestroyFrameBuffers(const IRenderInstance& _instance)
	{
		mSwapChain.DestroyFrameBuffers(_instance);
	}


	RenderSurface::SupportDetails RenderSurface::QuerySupportDetails(VkPhysicalDevice _device) const
	{
		SupportDetails details{};

		// KHR Capabilities.
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(_device, mHandle, &details.capabilities);


		// KHR Formats.
		uint32 formatCount = 0u;
		vkGetPhysicalDeviceSurfaceFormatsKHR(_device, mHandle, &formatCount, nullptr);

		if (formatCount != 0)
		{
			details.formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(_device, mHandle, &formatCount, details.formats.data());
		}


		// KHR Present Modes.
		uint32 presentModeCount = 0u;
		vkGetPhysicalDeviceSurfacePresentModesKHR(_device, mHandle, &presentModeCount, nullptr);

		if (presentModeCount != 0)
		{
			details.presentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(_device, mHandle, &presentModeCount, details.presentModes.data());
		}

		return details;
	}

	VkSurfaceFormatKHR RenderSurface::ChooseSwapSurfaceFormat(const SupportDetails& _details)
	{
		// Prefered
		for (uint32 i = 0; i < _details.formats.size(); ++i)
		{
			if (_details.formats[i].format == VK_FORMAT_B8G8R8A8_SRGB
				&& _details.formats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
				return _details.formats[i];
		}

		// Default.
		return _details.formats[0];
	}

	VkPresentModeKHR RenderSurface::ChooseSwapPresentMode(const SupportDetails& _details)
	{
		// Prefered.
		for (uint32 i = 0; i < _details.presentModes.size(); ++i)
		{
			if (_details.presentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR)
				return _details.presentModes[i];
		}

		// Default FIFO always supported.
		return VK_PRESENT_MODE_FIFO_KHR;
	}

	Vec2ui RenderSurface::ChooseSwapExtent(const SupportDetails& _details)
	{
		if (_details.capabilities.currentExtent.width != UINT32_MAX)
			return Vec2ui(_details.capabilities.currentExtent.width, _details.capabilities.currentExtent.height);
		else
		{
			// TODO: Clean.
			Vec2ui actualExtent = { 1920, 1080 };

			actualExtent.x = Maths::Clamp(actualExtent.x, _details.capabilities.minImageExtent.width, _details.capabilities.maxImageExtent.width);
			actualExtent.y = Maths::Clamp(actualExtent.y, _details.capabilities.minImageExtent.height, _details.capabilities.maxImageExtent.height);

			return actualExtent;
		}
	}


	RenderSurface::operator VkSurfaceKHR() const
	{
		return mHandle;
	}
}
