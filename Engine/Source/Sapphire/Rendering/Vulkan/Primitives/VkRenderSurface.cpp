// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Collections/Debug>

#include <Rendering/Vulkan/Primitives/VkRenderSurface.hpp>

#include <Rendering/Vulkan/VkRenderInstance.hpp>
#include <Rendering/Vulkan/Queue/VkQueueFamilyIndices.hpp>

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

	const VkRenderPass& VkRenderSurface::GetRenderPass() const
	{
		return mRenderPass;
	}

	void VkRenderSurface::Create(const VkDevice& _device, const VkQueueFamilyIndices& _queueFamilyIndices)
	{
		SA_ASSERT(mHandle != VK_NULL_HANDLE, Nullptr, Rendering,
			L"Handle is nullptr. VkSurfaceKHR must be created first: use VkRenderInstance.CreateRenderSurface().");

		mSwapChain.Create(_device, *this, _queueFamilyIndices);

		mRenderPass.Create(_device, mSwapChain.GetImageFormat());

		mSwapChain.CreateFrameBuffers(_device, mRenderPass);
	}

	void VkRenderSurface::Destroy(const VkDevice& _device)
	{
		SA_ASSERT(mHandle != VK_NULL_HANDLE, Nullptr, Rendering,
			L"Handle is nullptr. VkSurfaceKHR must be created first: use VkRenderInstance.CreateRenderSurface().");

		onResizeEvent.Clear();

		mSwapChain.DestroyFrameBuffers(_device);

		mRenderPass.Destroy(_device);

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
			// TODO: use Vec2<uint32>::Zero.
			Vec2<uint32>(), GetImageExtent(),
			ImageViewExtent{ Vec2<uint32>(), GetImageExtent() }
		};
	}


	void VkRenderSurface::ResizeCallback(const IRenderInstance& _instance, uint32 _width, uint32 _height)
	{
		const ImageExtent& extent = mSwapChain.GetImageExtent();

		// Avoid re-creation on same extent (ex: after window exiting minimization).
		if (extent.width == _width && extent.height == _height)
			return;

		IRenderSurface::ResizeCallback(_instance, _width, _height);

		const VkDevice& device = _instance.As<VkRenderInstance>().GetDevice();

		VkQueueFamilyIndices queueFamilyIndices;
		VkDevice::QueryQueueFamilies(device, *this, queueFamilyIndices);

		mSwapChain.DestroyFrameBuffers(device);

		mSwapChain.ReCreate(device, *this, queueFamilyIndices);

		mRenderPass.ReCreate(device, mSwapChain.GetImageFormat());

		mSwapChain.CreateFrameBuffers(device, mRenderPass);
	}

	VkRenderSurface::operator VkSurfaceKHR() const
	{
		return mHandle;
	}
}

#endif
