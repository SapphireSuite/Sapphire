// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Collections/Debug>

#include <Rendering/Vulkan/Primitives/VkRenderSurface.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa
{
	void VkRenderSurface::InitHandle(VkSurfaceKHR _newHandle)
	{
		SA_ASSERT(mHandle == VK_NULL_HANDLE, AlreadyCreated, Rendering, L"VkSurfaceKHR Handle already created.");

		mHandle = _newHandle;
	}

	void VkRenderSurface::UnInitHandle()
	{
		// Assume vkDestroySurfaceKHR has been called before (see VkRenderInstance.DestroyRenderSurface()).
		mHandle = nullptr;
	}

	VkSwapChain& VkRenderSurface::GetSwapChain()
	{
		return mSwapChain;
	}

	const VkSwapChain& VkRenderSurface::GetSwapChain() const
	{
		return mSwapChain;
	}

	void VkRenderSurface::Create(const VkDevice& _device, const VkQueueFamilyIndices& _queueFamilyIndices)
	{
		SA_ASSERT(mHandle != VK_NULL_HANDLE, Nullptr, Rendering,
			L"Handle is nullptr. VkSurfaceKHR must be created first: use VkRenderInstance.CreateRenderSurface().");

		mSwapChain.Create(_device, *this, _queueFamilyIndices);
	}

	void VkRenderSurface::Destroy(const VkDevice& _device)
	{
		SA_ASSERT(mHandle != VK_NULL_HANDLE, Nullptr, Rendering,
			L"Handle is nullptr. VkSurfaceKHR must be created first: use VkRenderInstance.CreateRenderSurface().");

		mSwapChain.Destroy(_device);
	}

	VkFormat VkRenderSurface::GetImageFormat() const noexcept
	{
		return mSwapChain.GetImageFormat();
	}

	const ImageExtent& VkRenderSurface::GetImageExtent() const noexcept
	{
		return mSwapChain.GetImageExtent();
	}

	Viewport VkRenderSurface::GetViewport() const noexcept
	{
		return Viewport
		{
			Vec2<uint32>::Zero, GetImageExtent(),
			Scissor{ Vec2<uint32>::Zero, GetImageExtent() }
		};
	}


	VkRenderSurface::operator VkSurfaceKHR() const
	{
		return mHandle;
	}
}

#endif
